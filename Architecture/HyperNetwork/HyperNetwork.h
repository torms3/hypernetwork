#ifndef _HYPERNETWORK_H_
#define _HYPERNETWORK_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "Node.h"
#include "Edge.h"
#include "Weight.h"
#include "ClassLabel.h"
#include <set>
#include <map>

using std::set;
using std::map;

#if defined( _TEST )
#include <fstream>
using std::ofstream;
#endif

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// HyperNetwork H = ( X, E, W )
	//
	class HyperNetwork
	{
	public:
							HyperNetwork();
		virtual				~HyperNetwork();

		virtual	void		Initialize();
		virtual void		Finalize();
		
		// Operations for nodes
		HNode				AddNode( Node& node );		
		
		// Operations for edges
		int					GetSize() const				{ return m_edge.size(); }
		HEdge				GetEdge( int idx )			{ return m_edge.at( idx ); }
		void				AddEdge( Edge* pEdge );
		void				RemoveEdge( HEdge e );

		// Remove unreferenced objects
		void				RemoveUnreferenced();

#if defined( _TEST )
		virtual void		TestFilePrint( ofstream& fout ) const;
#endif

	protected:
		// Operations for nodes
		void				RemoveUnreferencedNodes();

		// Operations for weights
		HWeight				AddWeight( Weight& weight );
		//void				RemoveWeight( HWeight w );
		void				RemoveUnreferencedWeights();
	
	protected:
		typedef set< HNode, LessForRefHandle<HNode> >	NodeSet;

	protected:
		NodeSet				m_node;
		vector< HEdge >		m_edge;
		vector< HWeight >	m_weight;
	};
}

#endif