//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "Value.h"
#include <stdexcept>

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for HValue class
	//
	HValue::HValue( IValue* val/* = 0*/ )
		: m_value( 0 )
	{
		if( val )
		{
			m_value = val->Clone();
		}
	}

	HValue::HValue( const HValue& hval )
		: m_value( 0 )
	{
		if( hval.m_value )
		{
			m_value = hval.m_value->Clone();
		}
	}

	HValue::~HValue()
	{
		delete m_value;
	}

	HValue& HValue::operator=( const HValue& rhs )
	{
		if( this != &rhs )
		{
			delete m_value;

			if( rhs.m_value )
			{
				m_value = rhs.m_value->Clone();
			}
			else
			{
				m_value = 0;
			}
		}

		return *this;
	}

	IValue& HValue::operator*() const
	{
		if( m_value )
		{
			return *m_value;
		}
		throw std::runtime_error( "unbound HValue" );
	}

	IValue* HValue::operator->() const
	{
		if( m_value )
		{
			return m_value;
		}
		throw std::runtime_error( "unbound HValue" );
	}

	bool HValue::operator==( const HValue& rhs ) const
	{
		if( m_value && rhs.m_value )
			return (*m_value == *(rhs.m_value));
		throw std::runtime_error( "unbound HValue" );
	}

	bool HValue::operator!=( const HValue& rhs ) const
	{
		if( m_value && rhs.m_value )
			return !(*this == rhs);
		throw std::runtime_error( "unbound HValue" );
	}

	bool HValue::operator<( const HValue& rhs ) const
	{
		if( m_value && rhs.m_value )
			return (*m_value < *rhs.m_value);
		throw std::runtime_error( "unbound HValue" );		
	}
}