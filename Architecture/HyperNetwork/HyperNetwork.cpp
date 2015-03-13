//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "HyperNetwork.h"

#if defined( _TEST )
using std::endl;
#endif

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for HyperNetwork class
	//
	HyperNetwork::HyperNetwork()
	{
		Initialize();
	}

	HyperNetwork::~HyperNetwork()
	{
		Finalize();
	}

	void HyperNetwork::Initialize()
	{
		m_node.clear();
		m_edge.clear();
		m_weight.clear();
	}

	void HyperNetwork::Finalize()
	{	
	}

	HNode HyperNetwork::AddNode( Node& node )
	{
		// Insert node and return the handle for further use
		return *((m_node.insert( HNode( node ) )).first);
	}

	void HyperNetwork::RemoveUnreferencedNodes()
	{
		NodeSet::iterator it = m_node.begin();
		while( it != m_node.end() )		
		{
			if( 1 == (*it).GetRefCnt() )
			{
				it = m_node.erase( it );
				continue;
			}

			++it;
		}
	}

	void HyperNetwork::AddEdge( Edge* pEdge )
	{
		if( pEdge )
		{
			// [04/18/2011 kslee]
			// TODO: Integrity maintenance feature should be added.

			// Add a weight for the edge
			Weight weight;
			HWeight hWeight = AddWeight( weight );
			pEdge->SetWeight( hWeight );

			m_edge.push_back( HEdge( pEdge ) );
		}
	}

	void HyperNetwork::RemoveEdge( HEdge e )
	{
		vector<HEdge>::iterator it = m_edge.begin();
		while( it != m_edge.end() )
		{
			if( e.GetDataPtr() == (*it).GetDataPtr() )
			{
				//HWeight hWeight = e->GetWeight();
				//RemoveWeight( hWeight );

				it = m_edge.erase( it );
				continue;
			}
		
			++it;
		}
	}

	void HyperNetwork::RemoveUnreferenced()
	{
		RemoveUnreferencedNodes();
		RemoveUnreferencedWeights();
	}

	HWeight HyperNetwork::AddWeight( Weight& weight )
	{
		HWeight hWeight( weight );
		m_weight.push_back( hWeight );
		return hWeight;
	}

	//void HyperNetwork::RemoveWeight( HWeight w )
	//{
	//	bool removed = false;
	//	vector<HWeight>::iterator it = m_weight.begin();
	//	while( it != m_weight.end() )
	//	{
	//		if( w.GetDataPtr() == (*it).GetDataPtr() )
	//		{
	//			it = m_weight.erase( it );
	//			removed = true;
	//			continue;
	//		}
	//		
	//		++it;
	//	}

	//	if( !removed )
	//		throw std::runtime_error( "no corresponding weight found when a edge is removed" );
	//}

	void HyperNetwork::RemoveUnreferencedWeights()
	{
		vector<HWeight>::iterator it = m_weight.begin();
		while( it != m_weight.end() )		
		{
			if( 1 == (*it).GetRefCnt() )
			{
				it = m_weight.erase( it );
				continue;
			}

			++it;
		}
	}

#if defined( _TEST )
	void HyperNetwork::TestFilePrint( ofstream& fout ) const
	{
		fout << "[[[[ HyperNetwork ]]]]" << endl;
		fout << "[" << endl;
		fout << endl;
		fout << "[# node: " << m_node.size() << "]" << endl;
		fout << "[# edge: " << m_edge.size() << "]" << endl;
		fout << "[# weight: " << m_weight.size() << "]" << endl;
		fout << endl;
		
		fout << "\t" << "[ Edges ]" << endl;
		fout << "\t" << "[" << endl;
		vector<HEdge>::const_iterator it = m_edge.begin();
		for( ; it != m_edge.end(); ++it )
		{
			fout << (*it)->TestString( "\t\t" );
		}
		fout << "\t" << "]" << endl;
		
		fout << "]" << endl;
	}
#endif
}
