//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "EdgeForEEGSpatioTemporal.h"
#include <cstring>
#include <assert.h>

#if defined( _TEST )
#include <sstream>
using std::ostringstream;
using std::endl;
#include <algorithm>
#endif

namespace HN
{
	//
	// Implementation for EEGEdge class
	//
	EEGEdge::EEGEdge()
	{
	}

	EEGEdge::~EEGEdge()
	{
	}

	void EEGEdge::Match( HData hData )
	{
		LearningEdge::Match( hData );
	}

	void EEGEdge::AddIndex(int t, int edgeIndex)
	{
		if( (int)m_indexTable.size() <= t )	m_indexTable.resize(t+1);
		m_indexTable[t].push_back(edgeIndex);
	}

	vector< int >& EEGEdge::GetNodeIndexList( int t )
	{
		assert( (int)m_indexTable.size() > t);
		return m_indexTable[t];
	}

	void EEGEdge::Print(ofstream& fout, int matched_count, int num, int signal_size, int time_size)
	{
		fout << "[Edge #" << num << "]" << endl;
		fout << "_ Matched Count:" << matched_count << endl;
		fout << "_ Weight: " << GetWeight() << endl;

		int cnt(1);
		for( int i=0; i<(int)m_indexTable.size(); ++i)
		{
			fout << "[";
			for( int j=0; j<(int)m_indexTable[i].size(); ++j)
			{
				int idx = m_indexTable[i][j];
				Node* node = dynamic_cast<Node*> ( GetNode(idx).GetDataPtr() );

				fout << node->GetIndex() / time_size;
				if(j!=m_indexTable[i].size()-1)	fout << ",";
				// fout << " (" << "TI:" << node->GetIndex() % time_size << ")  //  ";
				// [04/19/2011 hmlee]
				// Need Modification
				// fout << "Val:" << node->GetValue() << ")  //  ";
			}
			fout << "]";
		}

		fout << endl << endl;
	}

	bool EEGEdge::EqualsTo( const Edge& e ) const
	{
		return Edge::EqualsTo( e );
	}

	bool EEGEdge::LessThan( const Edge& e ) const
	{
		return Edge::LessThan( e );
	}

#if defined( _TEST )
	string EEGEdge::TestString( string format ) const
	{
		ostringstream sout;

		sout << "[length = " << GetTimeSize() << "]";
		for( int i = 0; i < GetTimeSize(); ++i )
		{
			sout << "[";
			
			vector<int> indexList = m_indexTable[i];
			std::sort( indexList.begin(), indexList.end() );
			vector<int>::const_iterator it = indexList.begin();
			for( ; it != indexList.end(); ++it )
			{
				sout << " " << GetNode( *it )->GetValue()->TestString();
			}

			sout << " ]";
		}

		return sout.str();
	}
#endif

}