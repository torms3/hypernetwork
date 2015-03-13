//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "ClassLabel.h"

#if defined( _TEST )
#include <sstream>
using std::ostringstream;
#endif

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for IntLabel class
	//
	bool Label::operator==( const Label& rhs ) const
	{
		return (m_label == rhs.m_label);
	}

	bool Label::operator!=( const Label& rhs ) const
	{
		return !(*this == rhs);
	}

	bool Label::operator<( const Label& rhs ) const
	{
		return (m_label < rhs.m_label);
	}

	bool Label::EqualsTo( const IValue& val ) const
	{
		Label* label = dynamic_cast<Label*>( const_cast<IValue*>( &val ) );
		if( label )
		{
			return (*this == *label);
		}
		else
		{
			return false;
		}
	}

	bool Label::LessThan( const IValue& val ) const
	{
		Label* label = dynamic_cast<Label*>( const_cast<IValue*>( &val ) );
		if( label )
		{
			return (*this < *label);
		}
		else
		{
			return false;
		}
	}

#if defined( _TEST )	
	string Label::TestString() const
	{
		ostringstream sout;
		sout << "[label: " << m_label << "]";
		return sout.str();
	}
#endif
}