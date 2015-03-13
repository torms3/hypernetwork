#ifndef _NODE_H_
#define _NODE_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "Value.h"
#include "RefHandle.h"

#if defined( _TEST )
#include <string>
using std::string;
#endif

namespace HN
{	
	//////////////////////////////////////////////////////////////////////////
	// Node X = { x_1, x_2, ..., x_l }
	//
	class Node
	{
	public:
							Node();
							Node( int index, HValue hVal );
		virtual				~Node();

		virtual bool		operator==( const Node& rhs ) const		{ return EqualsTo( rhs ); }
		virtual bool		operator!=( const Node& rhs ) const		{ return !EqualsTo( rhs ); }
		virtual bool		operator<( const Node& rhs ) const		{ return LessThan( rhs ); }

		HValue				GetValue()								{ return m_hValue; }
		void				SetValue( HValue hVal )					{ m_hValue = hVal; }

		int					GetIndex() const						{ return m_index; }
		void				SetIndex( int index )					{ m_index = index; }

#if defined( _TEST )
		string				TestString( string format ) const;
#endif

	protected:
		virtual bool		EqualsTo( const Node& rhs ) const;
		virtual bool		LessThan( const Node& rhs ) const;

	private:
		int					m_index;
		HValue				m_hValue;
	};

	//////////////////////////////////////////////////////////////////////////
	// Handle for Node class
	//	
	typedef RefHandle< Node >	HNode;
}

#endif