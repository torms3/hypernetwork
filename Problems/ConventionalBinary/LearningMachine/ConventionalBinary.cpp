//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "ConventionalBinary.h"
#include "EdgeForConventionalBinary.h"
#include "DataSetForConventionalBinary.h"

#include <ctime>
#include <algorithm>

#if defined( _TEST )
#include <fstream>
using std::ofstream;
#endif

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Initialize static constants for class ConvBinLM
	//
	const string	ConvBinLM::CONFIG_FILE_NAME = "config_convbin.xml";

	//////////////////////////////////////////////////////////////////////////
	// Implementation for ConventionalBinary class
	//
	ConvBinLM::ConvBinLM()
	{
		m_parameters.Load( "Config/" + CONFIG_FILE_NAME );
	}

	ConvBinLM::~ConvBinLM()
	{
	}

	void ConvBinLM::Initialize()
	{
		m_dataIndexForRemovedEdge.clear();
		
		// Set time seed for the random number generator
		m_rng.seed( static_cast<unsigned int>( time( 0 ) ) );

		// Calculate order distribution and sum
		CountOrder();
	}

	void ConvBinLM::Sampling()
	{
		// Mark existing edges as old
		for( int i = 0; i < m_library.GetSize(); ++i )
		{
			HEdge hEdge = m_library.GetEdge( i );
			ConvBinEdge* pEdge = dynamic_cast<ConvBinEdge*>( hEdge.GetDataPtr() );
			if( pEdge )
			{
				pEdge->SetNew( false );
			}
		}

		vector<int>::iterator it = m_dataIndexForRemovedEdge.begin();
		if( it != m_dataIndexForRemovedEdge.end() )	// Re-sampling
		{
			while( it != m_dataIndexForRemovedEdge.end() )
			{
				Sampling( *(it++) );
			}
			m_dataIndexForRemovedEdge.clear();
		}
		else // Make initial random library
		{
			for( int i = 0; i < m_trainingSet->GetNumData(); ++i )
			{
				Sampling( i );
			}
		}
	}

	void ConvBinLM::Matching()
	{
		for( int i = 0; i < m_trainingSet->GetNumData(); ++i )
		{
			m_library.MatchEdge( m_trainingSet->GetData( i ) );
		}
	}

	void ConvBinLM::Scoring()
	{
		// Calculate score
		CalcLibraryScore();

		// Calculate order distribution and sum
		CountOrder();

		// Sorting HyperNetwork edges by score
		m_library.SortEdge();
	}

	void ConvBinLM::Pruning()
	{		
		// Eliminate low score HyperNetwork edges
		EliminateBadEdgeByClass();

		// HyperNetwork garbage collection
		m_library.RemoveUnreferenced();
	}

	bool ConvBinLM::IsEnough()
	{
#if defined( _TEST )
		return (m_iteration >= 2);
#else
		return (m_iteration >= m_parameters.GetIntParam( "ITERATION" ));
#endif
		
	}

	void ConvBinLM::Finalize()
	{
#if defined( _TEST )
		TestFilePrint();
#endif
	}

	void ConvBinLM::InitOrderDistribution()
	{
		m_orderDistByClass.clear();

		ConvBinDataSet* dataSet = dynamic_cast<ConvBinDataSet*>( m_trainingSet.GetDataPtr() );
		if( dataSet )
		{
			vector<Label> labelSet;
			dataSet->GetLabelSet( labelSet );

			vector<Label>::const_iterator it = labelSet.begin();
			for( ; it != labelSet.end(); ++it )
			{
				OrderDist initOrderDist;
				for( int i = m_parameters.GetIntParam( "MIN_ORDER" ); i <= m_parameters.GetIntParam( "MAX_ORDER" ); ++i )
				{
					initOrderDist[i] = m_parameters.GetIntParam( "INIT_DIST" );
				}
				m_orderDistByClass[*it] = initOrderDist;
			}
		}
	}

	void ConvBinLM::CountOrder()
	{
		InitOrderDistribution();
		for( int i = 0; i < m_library.GetSize(); ++i )
		{
			HEdge edge = m_library.GetEdge( i );
			m_orderDistByClass[edge->GetLabel()][edge->GetOrder()]++;
		}
		
		m_orderSumByClass.clear();
		ConvBinDataSet* dataSet = dynamic_cast<ConvBinDataSet*>( m_trainingSet.GetDataPtr() );
		if( dataSet )
		{
			vector<Label> labelSet;
			dataSet->GetLabelSet( labelSet );

			vector<Label>::const_iterator it = labelSet.begin();
			for( ; it != labelSet.end(); ++it )
			{
				for( int i = m_parameters.GetIntParam( "MIN_ORDER" ); i <= m_parameters.GetIntParam( "MAX_ORDER" ); ++i )
				{
					m_orderSumByClass[*it] = m_orderDistByClass[*it][i];
				}
			}
		}
	}

	int ConvBinLM::GetSamplingOrder( Label label )
	{
#if defined( _TEST )
		return 5;
#endif
		boost::uniform_int<> dimension( 0, m_orderSumByClass[label] );
		boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die( m_rng, dimension );
		int random = die();
		if( random == m_orderSumByClass[label] )
			random--;
		
		int maxOrder = m_parameters.GetIntParam( "MAX_ORDER" );
		if( m_trainingSet->GetDimension() < m_parameters.GetIntParam( "MAX_ORDER" ) )
			maxOrder = m_trainingSet->GetDimension();

		int order = m_parameters.GetIntParam( "MIN_ORDER" );
		int sum = 0;
		for( int i = m_parameters.GetIntParam( "MIN_ORDER" ); i <= maxOrder; ++i )
		{
			sum += m_orderDistByClass[label][i];

			if( random < sum )
			{
				order = i;
				break;
			}
		}

		return order;
	}

	void ConvBinLM::Sampling( int dataIndex )
	{
		HData data = m_trainingSet->GetData( dataIndex );
		
		// Edge Sampling
		vector< int > orderList;
		{
			orderList.resize( data->GetDimension() );

			for( int iter = 0 ; iter < m_parameters.GetIntParam( "SAMPLE_RATE" ); ++iter )
			{
				for( int i = 0; i < data->GetDimension(); ++i )
					orderList[i] = i;

				ConvBinEdge edge;
				int order = GetSamplingOrder( data->GetLabel() );

				// Edge setting
				for( int i = 0; i < order; ++i )
				{
					boost::uniform_int<> dimension( 0, data->GetDimension() - i - 1 );
					boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die( m_rng, dimension );
					int selectedIndex = die();

					HNode hNode = m_library.AddNode( data->GetNode( orderList[selectedIndex] ) );
					edge.AddNode( hNode );

					std::swap( orderList[selectedIndex], orderList[data->GetDimension() - i - 1] );
				}
				edge.SetLabel( data->GetLabel() );

				// ConvBinEdge setting
				edge.SetDataIndex( dataIndex );
				edge.SetNew( true );
				edge.SetTrainingSet( m_trainingSet );

				// Add the edge to the library
				m_library.AddEdge( &edge );
			}
		}
	}

	void ConvBinLM::CalcLibraryScore()
	{
		m_library.ScoreEdge();
	}

	void ConvBinLM::EliminateBadEdgeByClass()
	{
		m_dataIndexForRemovedEdge.clear();
		
		map<Label, vector<HEdge> > edgeByClass;
		m_library.SeparateEdgesByClass( edgeByClass );

		map<Label, vector<HEdge> >::iterator it = edgeByClass.begin();
		for( ; it != edgeByClass.end(); ++it )
		{
			vector<HEdge>& vec = (*it).second;

			int size = static_cast<int>( vec.size() );
			int amnt = static_cast<int>( static_cast<double>( size ) * GetMutationRatio( m_iteration ) );
			if( amnt == size )
				amnt--;

#if defined( _TEST )
			// [04/18/2011 kslee]
			// temp
			//amnt = 1;
#endif

			// [04/18/2011 kslee]
			// [[Complexity Issue]]
			// Extensive linear search and expansive erase operation are used here.
			// Should consider the alternative implementation, 
			// such as the original implementation.
			for( int i = 0; i < amnt; ++i )
			{
				HEdge hEdge = vec.back();
				ConvBinEdge* pEdge = dynamic_cast<ConvBinEdge*>( hEdge.GetDataPtr() );
				m_dataIndexForRemovedEdge.push_back( pEdge->GetDataIndex() );				
				
				m_library.RemoveEdge( hEdge );
				vec.pop_back();
			}
		}
	}

#if defined( _TEST )
	void ConvBinLM::TestFilePrint() const
	{
		ofstream fout( "ConventionalBinary.txt" );
		m_library.TestFilePrint( fout );
		fout.close();
	}
#endif

	double ConvBinLM::GetMutationRatio( int iter )
	{
		double numerator = m_parameters.GetDoubleParam( "MAX_REPLACE_RATIO" )  - m_parameters.GetDoubleParam( "MIN_REPLACE_RATIO" );
		double denominator = exp( static_cast<double>( (iter - 1) / m_parameters.GetIntParam("CONV_SPEED" ) ) );
		
		return ((numerator / denominator) + m_parameters.GetDoubleParam( "MIN_REPLACE_RATIO" ));
	}
}