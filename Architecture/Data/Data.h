#ifndef _DATA_H_
#define _DATA_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "Node.h"
#include "ClassLabel.h"
#include "RefHandle.h"
#include <vector>

using std::vector;

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Example Data Pattern Xn = (Xn_1, Xn_2,Xn_3, ... )
	//
	class Data
	{
	public:
		template< typename T >
		friend T*		Clone( const T* data );

	public:
						Data()								{ m_node.clear(); }
		virtual			~Data(){}

		Node&			GetNode( int index )				{ return m_node.at( index ); }
		void			SetNode( int index, Node& node )	{ m_node[index] = node; }	// For optimization
		void			AddNode( Node& node )				{ m_node.push_back( node ); }

		int				GetDimension() const				{ return m_node.size(); }
		void			SetDimension( int dim )				{ m_node.resize( dim ); }	// For optimization

		Label			GetLabel() const					{ return m_label; }
		void			SetLabel( Label& label )			{ m_label = label; }

	protected:
		virtual Data*	Clone() const						{ return new Data( *this ); }

	private:
		vector< Node >	m_node;
		Label			m_label;
	};

	//////////////////////////////////////////////////////////////////////////
	// Reference handle for Data class
	//
	typedef RefHandle< Data >	HData;
}

#endif