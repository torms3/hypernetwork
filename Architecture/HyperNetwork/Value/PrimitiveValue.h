#ifndef _PRIMITIVE_VALUE_H_
#define _PRIMITIVE_VALUE_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "Value.h"

#if defined( _TEST )
#include <sstream>
using std::ostringstream;
#endif

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Represents primitive value 
	// associated with each HyperNetwork node.
	//
	template< typename T >
	class PrimitiveValue : public IValue
	{
	public:
							PrimitiveValue( T val = 0 ) : m_value( val ) {}
		virtual				~PrimitiveValue() {}

		bool				operator==( const PrimitiveValue<T>& rhs ) const;
		bool				operator!=( const PrimitiveValue<T>& rhs ) const;
		bool				operator<( const PrimitiveValue<T>& rhs ) const;

		T					GetValue() const	{ return m_value; }
		void				SetValue( T val )	{ m_value = val; }

#if defined( _TEST )
		virtual string		TestString() const;
#endif

	protected:
		virtual IValue*		Clone() const		{ return new PrimitiveValue<T>( *this ); }
		virtual bool		EqualsTo( const IValue& rhs ) const;
		virtual bool		LessThan( const IValue& rhs ) const;

	private:
		T					m_value;
	};

	//////////////////////////////////////////////////////////////////////////
	// Implementation of PrimitiveValue<T> class
	//
	template< typename T >
	bool PrimitiveValue<T>::operator==( const PrimitiveValue<T>& rhs ) const
	{
		return (m_value == rhs.m_value);
	}

	template< typename T >
	bool PrimitiveValue<T>::operator!=( const PrimitiveValue<T>& rhs ) const
	{
		return !(*this == rhs);
	}

	template< typename T >
	bool PrimitiveValue<T>::operator<( const PrimitiveValue<T>& rhs ) const
	{
		return (m_value < rhs.m_value);
	}

	template< typename T >
	bool PrimitiveValue<T>::EqualsTo( const IValue& rhs ) const
	{
		PrimitiveValue<T>* pval = dynamic_cast<PrimitiveValue<T>*>( const_cast<IValue*>( &rhs ) );
		if( pval )
		{
			return (*this == *pval);
		}
		else
		{
			return false;
		}
	}

	template< typename T >
	bool PrimitiveValue<T>::LessThan( const IValue& rhs ) const
	{
		PrimitiveValue<T>* pval = dynamic_cast<PrimitiveValue<T>*>( const_cast<IValue*>( &rhs ) );
		if( pval )
		{
			return (*this < *pval);
		}
		else
		{
			return false;
		}
	}

#if defined( _TEST )
	template< typename T >
	string PrimitiveValue<T>::TestString() const
	{
		ostringstream sout;
		sout << m_value;
		return sout.str();
	}
#endif
}

#endif