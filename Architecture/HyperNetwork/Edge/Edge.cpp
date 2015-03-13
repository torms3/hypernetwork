//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "Edge.h"

#if defined( _TEST )
#include <sstream>
using std::ostringstream;
using std::endl;
#endif

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for Edge class
	//
	Edge::Edge()
	{
		m_node.clear();
		m_label.SetValue( 0 );
	}

	Edge::~Edge()
	{
	}

	int Edge::AddNode( HNode& hNode )
	{
		m_node.push_back( hNode );	
		return m_node.size() - 1;
	}

#if defined( _TEST )
	string Edge::TestString( string format ) const
	{
		ostringstream sout;
		sout << format << "[[[ Edge ]]]" << endl;
		sout << format << "[" << endl;
		sout << format << endl;
		sout << format << "[# node: " << m_node.size() << "]" << endl;
		sout << format << endl;

		sout << format << "\t" << "[ Nodes ]" << endl;
		sout << format << "\t" << "[" << endl;
		vector<HNode>::const_iterator it = m_node.begin();
		for( ; it != m_node.end(); ++it )
		{
			sout << (*it)->TestString( format + "\t\t" );
		}
		sout << format << "\t" << "]" << endl;

		sout << format << "]" << endl;

		return sout.str();
	}
#endif
}