//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "Node.h"

#if defined( _TEST )
#include <sstream>
using std::ostringstream;
using std::endl;
#endif

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for Node class
	//
	Node::Node()
		: m_index( 0 )
	{
	}

	Node::Node( int index, HValue hVal )
		: m_index( index ), m_hValue( hVal )
	{
	}

	Node::~Node()
	{
	}

	bool Node::EqualsTo( const Node& rhs ) const
	{
		return (m_index == rhs.m_index &&
				m_hValue == rhs.m_hValue);
	}

	bool Node::LessThan( const Node& rhs ) const
	{
		if( m_index < rhs.m_index )
		{
			return true;
		}
		else if( m_index == rhs.m_index )
		{
			if( m_hValue < rhs.m_hValue )
			{
				return true;
			}
		}

		return false;
	}

#if defined( _TEST )
	string Node::TestString( string format ) const
	{
		ostringstream sout;
		sout << format << "[[ Node ]]" << endl;
		sout << format << "[" << endl;
		sout << format << "\t" << "[index: " << m_index << "]" << endl;
		sout << format << "\t" << "[value: " << m_hValue->TestString() << "]" << endl;
		sout << format << "]" << endl;
		return sout.str();
	}
#endif
}