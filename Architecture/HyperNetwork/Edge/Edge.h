#ifndef _EDGE_H_
#define _EDGE_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "RefHandle.h"
#include "Node.h"
#include "Weight.h"
#include "ClassLabel.h"
#include <vector>

using std::vector;

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Edge E = { E_1, E_2, ..., E_|E| }
	//
	class Edge
	{
	public:
		template< typename T >
		friend T*			Clone( const T* data );

	public:
							Edge();
		virtual				~Edge();

		bool				operator==( const Edge& rhs ) const	{ return EqualsTo( rhs ); }
		bool				operator!=( const Edge& rhs ) const	{ return !EqualsTo( rhs ); }
		bool				operator<( const Edge& rhs ) const	{ return LessThan( rhs ); }

		// Operations for nodes
		HNode				GetNode( int index ) const			{ return m_node.at( index ); }		
		int					AddNode( HNode& hNode );
		int					GetOrder() const					{ return m_node.size(); }

		// Operations for the weight
		HWeight				GetWeight()	const					{ return m_hWeight; }
		void				SetWeight( HWeight& w )				{ m_hWeight = w; }

		// Operations for the label
		Label				GetLabel() const					{ return m_label; }
		void				SetLabel( Label& label )			{ m_label = label; }

#if defined( _TEST )
		virtual string		TestString( string format ) const;
#endif

	protected:
		virtual Edge*		Clone() const						{ return new Edge( *this ); }
		virtual bool		EqualsTo( const Edge& e ) const		{ return (m_hWeight == e.m_hWeight); }
		virtual bool		LessThan( const Edge& e ) const		{ return (m_hWeight < e.m_hWeight); }		

	protected:
		vector< HNode >		m_node;
		HWeight				m_hWeight;
		Label				m_label;
	};

	//////////////////////////////////////////////////////////////////////////
	// Handle for Edge class
	//
	typedef RefHandle< Edge >	HEdge;
}

#endif