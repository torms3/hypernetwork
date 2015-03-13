//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

// #define EEG_DEBUG

#include "EEGSpatioTemporalLM.h"
#include <ctime>
#include <algorithm>
#include <assert.h>
#include <fstream>
using namespace std;

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Initialize static constants for class EEGSpatioTemporal
	//
	const string	EEGSpatioTemporalLM::CONFIG_FILE_NAME = "config_eegspatiotemporal.xml";
	const string	EEGSpatioTemporalLM::OUTPUT_FILE_NAME = "output_eegspatiotemporal.txt";

	//////////////////////////////////////////////////////////////////////////
	// Implementation for EEGSpatioTemporal class
	//
	EEGSpatioTemporalLM::EEGSpatioTemporalLM()
	{
		m_parameters.Load( "Config/" + CONFIG_FILE_NAME );
	}

	EEGSpatioTemporalLM::~EEGSpatioTemporalLM()
	{
	}

	void EEGSpatioTemporalLM::Initialize()
	{
		m_fout.open( OUTPUT_FILE_NAME );
#if defined( _TEST )
		m_fout2.open( "EEG_debug_result.txt" );
#endif
	}

	void EEGSpatioTemporalLM::Training()
	{
		m_isTraining = true;
		m_iteration = 0;

		Initialize();

		// Training loop
		do
		{
			m_iteration++;

			Sampling();
			Merging();
			Scoring();
			Pruning();

		} while( !IsEnough() );

		Matching();

		Finalize();

		m_isTraining = false;
	}

	void EEGSpatioTemporalLM::Sampling()
	{	
		// It could need to get rng as an argument from the outside.
		// Such as : Sampling( rng )
		boost::mt19937 rng;
		rng.seed( static_cast<unsigned int>( time( 0 ) ) );

		for( int dataIndex = 0; dataIndex < m_trainingSet->GetNumData(); ++dataIndex )
		{
			EEGData* data = dynamic_cast<EEGData*>( m_trainingSet->GetData( dataIndex ).GetDataPtr() );
			if( data )
			{
				int signal_size = data->GetSignalSize();
				int time_size = data->GetTimeSize();
				m_signal_size = signal_size, m_time_size = time_size;

				int signalOrder = GetSamplingSignalOrder( data->GetLabel().GetValue() );
				int timeOrder = GetSamplingTimeOrder( data->GetLabel().GetValue() );

				//
				// [04/18/2011 hmlee]
				// Current Basic Sampling method
				// 1. Select random time [t] from time interval.
				// 2. Select random (# of SamplingSignalOrder) signals from each time t ~ (t+SamplingTimeOrder-1).
				// 
#if defined( _TEST )
				//for( int t_iter = 0; t_iter < time_size - 1; ++t_iter )
#endif
				for( int iter = 0 ; iter < m_parameters.GetIntParam( "SAMPLE_RATE" ); ++iter )
				{
					EEGEdge edge;

#if defined( _TEST )
					//int selectedTime = t_iter;
					int selectedTime = GetRandomValue(rng, time_size - timeOrder);
#else
					int selectedTime = GetRandomValue(rng, time_size - timeOrder);
#endif

					for( int t = selectedTime; t < selectedTime + timeOrder; ++t )
					{
						vector< int > selectedSignalList = GetRandomValueList(rng, signal_size, signalOrder);

						for( int i = 0; i < (int)selectedSignalList.size(); ++i )
						{
							int dataIndex = data->GetIndex(selectedSignalList[i], t );
							HNode hNode = m_library.AddNode( data->GetNode( dataIndex ) );

							int edgeIndex = edge.AddNode( hNode );
							edge.AddIndex(t - selectedTime, edgeIndex);
						}
					}

					m_library.AddEdge( &edge );
					m_library.GetEdge( m_library.GetSize() - 1 )->GetWeight()->SetWeight( edge.GetTimeSize() );

					// For debug
					// edge.Print(m_fout, 1, 1, m_signal_size, m_time_size);
				}
			}
		}
	}

	void EEGSpatioTemporalLM::Merging()
	{
		if(m_library.GetSize() < 2)	return;

		boost::mt19937 rng;
		rng.seed( static_cast<unsigned int>( time( 0 ) ) );

		int ITERATE_RATE = m_parameters.GetIntParam( "MERGE_ITERATE_RATE" );
		int COMPARING_EDGES_FOR_ONE_ITERATE = m_parameters.GetIntParam( "MERGE_COMPARING_EDGES_FOR_ONE_ITERATE" );

		for( int iter = 0; iter < ITERATE_RATE; ++iter )
		{
			vector< EEGEdge > mergedEdgeList;
			
			for( int subIter = 0; subIter < COMPARING_EDGES_FOR_ONE_ITERATE; ++subIter )
			{
#if defined( _TEST )
				cout << "ITERATION: " << m_iteration << ", Merge: " << iter << ", SubMurge: " << subIter << endl;
#endif

				vector< int > edgeIndexList = GetRandomValueList(rng, m_library.GetSize(), 2);
				assert( edgeIndexList.size() == 2);

				EEGEdge* e0 = dynamic_cast<EEGEdge*> ( m_library.GetEdge(edgeIndexList[0]).GetDataPtr() );
				EEGEdge* e1 = dynamic_cast<EEGEdge*> ( m_library.GetEdge(edgeIndexList[1]).GetDataPtr() );

				bool isSame = CompareTwoEdge( e0, e1 );

				if(isSame)
				{
					EEGEdge edge = MergeTwoEdge( e0, e1 );
					mergedEdgeList.push_back( edge );
				}
			}

			for( int i = 0; i < (int)mergedEdgeList.size(); ++i )
			{				
				m_library.AddEdge( &mergedEdgeList[i] );
				m_library.GetEdge( m_library.GetSize() - 1 )->GetWeight()->SetWeight( mergedEdgeList[i].GetTimeSize() );
			}
		}
	}

	// [04/24/2011 hmlee]
	// TODO: Make NodeForEEGSpatioTemporal
	// TODO: Make Comparing Node Function in NodeForEEGSpatioTemporal
			
	bool EEGSpatioTemporalLM::CompareTwoEdge( EEGEdge* e0, EEGEdge* e1 )
	{
		// Basic Edge Comparing.

		double ACCEPTED_RATIO = m_parameters.GetDoubleParam( "ACCEPTED_RATIO" );

		int e0Size = e0->GetTimeSize();
		int e1Size = e1->GetTimeSize();

 		int compareSize = std::min(e0Size, e1Size) - 1;
 		if( compareSize < 1 )	return false;
 
 		bool same(true);

 		for( int i = 0; i < compareSize; ++i )
 		{
 			int j = e0Size - compareSize + i;
			
			vector< int > e0NodeIndexList = e0->GetNodeIndexList(j);
			vector< int > e1NodeIndexList = e1->GetNodeIndexList(i);

			int minIndexSize = min( e0NodeIndexList.size(), e1NodeIndexList.size() );
			int minimumAcceptedSameSignal = 
				(ACCEPTED_RATIO > 1.f - 1e-10) ? minIndexSize : int( minIndexSize * ACCEPTED_RATIO ) + 1;

			int sameSignal(0);

			for( int ii = 0; ii < (int)e0NodeIndexList.size(); ++ii )
			{
				for( int jj = 0; jj < (int)e1NodeIndexList.size(); ++jj )
				{
					Node* n0 = dynamic_cast<Node*> ( e0->GetNode( e0NodeIndexList[ii] ).GetDataPtr() );
					Node* n1 = dynamic_cast<Node*> ( e1->GetNode( e1NodeIndexList[jj] ).GetDataPtr() );

					// [04/19/2011 hmlee]
					// Comparing Nodes' Signals.
					// If same, Comparing each Nodes.
					if( GetSignalIndex(n0->GetIndex()) == GetSignalIndex(n1->GetIndex()) )
					{
						sameSignal++;
						if( n0->GetValue() != n1->GetValue() )
						{
							same = false;
							break;
						}
					}
				}
				if( ! same )	break;
			}
			if( ! same )	break;

			if( sameSignal < minimumAcceptedSameSignal )
			{
				same = false;
				break;
			}
		}
		return same;
	}

	EEGEdge EEGSpatioTemporalLM::MergeTwoEdge( EEGEdge* e0, EEGEdge* e1 )
	{
		// Basic Edge Merging.

		EEGEdge edge;

		int e0Size = e0->GetTimeSize();
		int e1Size = e1->GetTimeSize();

		int compareSize = std::min(e0Size, e1Size) - 1;

		for( int i = 0; i < e0Size - compareSize; ++i )
		{
			vector< int > e0NodeIndexList = e0->GetNodeIndexList(i);
			for( int ii = 0; ii < (int)e0NodeIndexList.size(); ++ii )
			{
				HNode hNode = e0->GetNode( e0NodeIndexList[ii] );
				int edgeIndex = edge.AddNode( hNode );
				edge.AddIndex(i, edgeIndex);
			}
		}

		for( int i = 0; i < compareSize; ++i )
		{
			int j = e0Size - compareSize + i;

			vector< int > e0NodeIndexList = e0->GetNodeIndexList(j);
			vector< int > e1NodeIndexList = e1->GetNodeIndexList(i);
			for( int ii = 0; ii < (int)e0NodeIndexList.size(); ++ii )
			{
				bool same(false);
				for( int jj = 0; jj < (int)e1NodeIndexList.size(); ++jj )
				{
					Node* n0 = dynamic_cast<Node*> ( e0->GetNode( e0NodeIndexList[ii] ).GetDataPtr() );
					Node* n1 = dynamic_cast<Node*> ( e1->GetNode( e1NodeIndexList[jj] ).GetDataPtr() );

					if( GetSignalIndex(n0->GetIndex()) == GetSignalIndex(n1->GetIndex()) )
					{
						same = true;
						break;
					}
				}
				if( same )	continue;

				HNode hNode = e0->GetNode( e0NodeIndexList[ii] );
				int edgeIndex = edge.AddNode( hNode );
				edge.AddIndex(j, edgeIndex);
			}

			for( int jj = 0; jj < (int)e1NodeIndexList.size(); ++jj )
			{
				HNode hNode = e1->GetNode( e1NodeIndexList[jj] );
				int edgeIndex = edge.AddNode( hNode );
				edge.AddIndex(j, edgeIndex);
			}
		}

		for( int j = compareSize; j < e1Size; ++j )
		{
			vector< int > e1NodeIndexList = e1->GetNodeIndexList(j);
			for( int jj = 0; jj < (int)e1NodeIndexList.size(); ++jj )
			{
				HNode hNode = e1->GetNode( e1NodeIndexList[jj] );
				int edgeIndex = edge.AddNode( hNode );
				edge.AddIndex(e0Size - compareSize + j, edgeIndex);
			}
		}
		
		return edge;
	}

	void EEGSpatioTemporalLM::Scoring()
	{
		// Calculate score
		CalcLibraryScore();

		// Sorting HyperNetwork edges by score
		m_library.SortEdge();
	}

	void EEGSpatioTemporalLM::Pruning()
	{		
		EliminateBadEdge();

		// HyperNetwork garbage collection
		m_library.RemoveUnreferenced();
	}

	bool EEGSpatioTemporalLM::IsEnough()
	{
		return m_iteration == m_parameters.GetIntParam( "ITERATION" );
	}
	
	void EEGSpatioTemporalLM::Matching()
	{
		if( m_trainingSet->GetNumData() == 0 )	return;

		int cnt(1);

		EEGData* data = dynamic_cast<EEGData*>( m_trainingSet->GetData( 0 ).GetDataPtr() );
		if( data )
		{
			for( int i = 0 ; i < m_library.GetSize(); ++i )
			{
				EEGEdge* edge = dynamic_cast<EEGEdge*> ( m_library.GetEdge(i).GetDataPtr() );
				int matched_count = Matched( edge, data );
				if( matched_count > 0 )
				{
					edge->Print(m_fout, matched_count, cnt++, m_signal_size, m_time_size);
#if defined( _TEST )
					m_fout2 << "[Matched count = " << matched_count << "]";
					m_fout2 << edge->TestString( "" ) << endl;
#endif
				}
			}
		}
	}

	int EEGSpatioTemporalLM::Matched( EEGEdge* edge, EEGData* data )
	{
		int signal_size = data->GetSignalSize();
		int time_size = data->GetTimeSize();
		int edge_time_size = edge->GetTimeSize();

		int matched_count(0);

		for( int t = 0; t < time_size - edge_time_size; ++t )
		{
			bool allSame(true);

			for( int dt = 0; dt < edge_time_size; ++dt )
			{
				int ct = t + dt;

				vector< int > edgeNodeIndexList = edge->GetNodeIndexList(dt);

				for( int i = 0; i < (int)edgeNodeIndexList.size(); ++i )
				{
					Node* node = dynamic_cast<Node*> ( edge->GetNode( edgeNodeIndexList[i] ).GetDataPtr() );
					int signalIndex = GetSignalIndex( node->GetIndex() );
					HNode dataNode = data->GetNode( data->GetIndex(signalIndex, ct) );
					if( node->GetValue() != dataNode->GetValue() )
					{
						allSame = false;
						break;
					}
				}
				if( ! allSame )	break;
			}

			if(allSame)
			{
				matched_count++;
			}
		}
		
		return matched_count;
	}

	void EEGSpatioTemporalLM::Finalize()
	{
		m_fout.close();
#if defined( _TEST )		
		m_fout2.close();
#endif
	}

	// [04/18/2011 hmlee]
	// This machine samples (Signal Order * TimeOrder) values.
	// No Label.
	int EEGSpatioTemporalLM::GetSamplingSignalOrder( int classLabel )
	{
		return 3;
	}
	int EEGSpatioTemporalLM::GetSamplingTimeOrder( int classLabel )
	{	
		return 2;
	}

	void EEGSpatioTemporalLM::CalcLibraryScore()
	{
		m_library.ScoreEdge();
	}

	void EEGSpatioTemporalLM::EliminateBadEdge()
	{
		int size = static_cast<int>( m_library.GetSize() );
		int amnt = static_cast<int>( static_cast<double>( size ) * GetMutationRatio( m_iteration ) );
		if( amnt == size )
			amnt--;

		for( int i = 0; i < amnt; ++i )
		{	
			// [04/29/2011 kslee]
			// For optimization
			m_library.RemoveLastEdge();
		}
	}

	int EEGSpatioTemporalLM::GetRandomValue(boost::mt19937& rng, int limit)
	{
		boost::uniform_int<> uniform_value( 0, limit - 1 );
		boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die( rng, uniform_value );
		return die();
	}
	vector< int > EEGSpatioTemporalLM::GetRandomValueList(boost::mt19937& rng, int limit, int number)
	{
		assert( limit >= number );
		vector< int > randomValueList;

		vector< int > orderList( limit );
		for( int i = 0; i < limit; ++i )
			orderList[i] = i;

		for( int i = 0; i < number; ++i )
		{
			int val = GetRandomValue(rng, limit - i);
			randomValueList.push_back( val );

			assert( val < limit );
			assert( limit - i - 1 < limit );
			
			std::swap( orderList[val], orderList[limit - i - 1] );
		}

		return randomValueList;
	}

	double EEGSpatioTemporalLM::GetMutationRatio( int iter )
	{
		double numerator = m_parameters.GetDoubleParam( "MAX_REPLACE_RATIO" )  - m_parameters.GetDoubleParam( "MIN_REPLACE_RATIO" );
		double denominator = exp( static_cast<double>( (iter - 1) / m_parameters.GetIntParam("CONV_SPEED" ) ) );

		return ((numerator / denominator) + m_parameters.GetDoubleParam( "MIN_REPLACE_RATIO" ));
	}

#if defined( _TEST )
	void EEGSpatioTemporalLM::TestFilePrint() const
	{
		ofstream fout( "EEGSpatioTmporal.txt" );
		m_library.TestFilePrint( fout );
		fout.close();
	}
#endif
}