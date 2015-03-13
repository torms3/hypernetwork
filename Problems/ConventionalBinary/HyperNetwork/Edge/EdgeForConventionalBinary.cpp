//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "EdgeForConventionalBinary.h"
#include "DataSetForConventionalBinary.h"
#include <cstring>

#if defined( _TEST )
#include <sstream>
using std::ostringstream;
using std::endl;
#endif

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for ConvBinEdge class
	//
	void ConvBinEdge::Match( HData hData )
	{
		if( IsNew() )
		{
			LearningEdge::Match( hData );
		}
	}

	void ConvBinEdge::Score()
	{
		//le.class_weight.resize( m_sizeCls );
		
		ConvBinDataSet* dataSet = dynamic_cast<ConvBinDataSet*>( m_trainingSet.GetDataPtr() );
		if( !dataSet )
			return;
		
		// [0] for edge class count
		// [1] for data set class count
		int myClassCnt[2] = { 0, };
		int otherClassCnt[2] = { 0, };

		map<Label, int>::const_iterator edge_it = m_data.classCount.begin();
		while( edge_it != m_data.classCount.end() )
		{
			if( edge_it->first == GetLabel() )
			{
				myClassCnt[0] = edge_it->second;
			}
			else
			{
				otherClassCnt[0] += edge_it->second;
			}

			++edge_it;
		}

		vector<Label> labelSet;
		dataSet->GetLabelSet( labelSet );
		vector<Label>::const_iterator labelSet_it = labelSet.begin();
		while( labelSet_it != labelSet.end() )
		{
			Label label = *labelSet_it;
			if( label == GetLabel() )
			{
				myClassCnt[1] = dataSet->GetClassCount( label );
			}
			else
			{
				otherClassCnt[1] += dataSet->GetClassCount( label );
			}

			++labelSet_it;
		}

		// Safeguard for division
		if( (0 == myClassCnt[1]) || (0 == otherClassCnt[1]) )
			return;

		double gap_my_term		= (myClassCnt[0] * static_cast<double>( m_trainingSet->GetNumData() ) / myClassCnt[1]);
		double gap_other_term	= (otherClassCnt[0] * static_cast<double>( m_trainingSet->GetNumData() ) / otherClassCnt[1]);
		double gap = gap_my_term - gap_other_term;

		double alpha_term	= ALPHA / ((otherClassCnt[0] + 1) * (otherClassCnt[0] + 1));
		double beta_term	= BETA * gap;
		double gamma_term	= GAMMA / GetOrder();

		m_data.score = alpha_term + beta_term + gamma_term;

		if( m_data.score < 0.0 )
			m_data.score = 0.0;
	
		// Replace weight by score
		m_hWeight->SetWeight( static_cast<int>( m_data.score ) );
	}

	void ConvBinEdge::Update( bool match, HData hData )
	{
		if( match )
		{
			m_data.classCount[hData->GetLabel()]++;

			if( GetLabel() == hData->GetLabel() )
			{
				m_hWeight->IncWeight( 1 );
			}
			else
			{
				m_hWeight->IncWeight( 1 );
			}
		}
	}

#if defined( _TEST )
	string ConvBinEdge::TestString( string format ) const
	{
		ostringstream sout;
		sout << format << "[[[ ConvBinEdge ]]]" << endl;
		sout << format << "[" << endl;
		sout << format << endl;
		sout << format << "\t" << "[# node: " << GetOrder() << "]" << endl;
		sout << format << "\t" << GetLabel().TestString() << endl;
		sout << format << endl;

		sout << format << "\t" << "[ Nodes ]" << endl;
		sout << format << "\t" << "[" << endl;
		for( int i = 0; i < GetOrder(); ++i )
		{
			sout << GetNode( i )->TestString( format + "\t\t" );
		}
		sout << format << "\t" << "]" << endl;
		
		sout << m_data.TestString( format + "\t" );

		sout << format << "]" << endl;
		

		return sout.str();
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Implementation for ConvBinEdge::Data class
	//
#if defined( _TEST )
	string ConvBinEdge::Data::TestString( string format ) const
	{
		ostringstream sout;
		sout << format << "[[ ConvBinEdge::Data ]]" << endl;
		sout << format << "[" << endl;
		sout << format << "\t" << "[dataIndex: " << dataIndex << "]" << endl;
		sout << format << "]" << endl;
		return sout.str();
	}
#endif
}