#ifndef _VALUE_H_
#define _VALUE_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#if defined( _TEST )
#include <string>
using std::string;
#endif

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Interface of Value class family.
	// Represents any value associated with each HyperNetwork node. 
	//
	class IValue
	{
	public:
		friend class HValue;

	public:
							IValue(){}
		virtual				~IValue(){}

		bool				operator==( const IValue& val ) const	{ return EqualsTo( val ); }
		bool				operator!=( const IValue& val ) const	{ return !EqualsTo( val ); }
		bool				operator<( const IValue& val ) const	{ return LessThan( val ); }

#if defined( _TEST )
		virtual string		TestString() const = 0;
#endif

	protected:
		virtual IValue*		Clone() const = 0;
		virtual bool		EqualsTo( const IValue& val ) const = 0;
		virtual bool		LessThan( const IValue& val ) const = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// Handle of IValue interface.
	//
	class HValue
	{
	public:
							HValue( IValue* val = 0 );
							HValue( const HValue& hval );
							~HValue();

							operator bool() const	{ return (m_value ? true : false); }
		HValue&				operator=( const HValue& rhs );
		IValue&				operator*() const;
		IValue*				operator->() const;
		bool				operator==( const HValue& rhs ) const;
		bool				operator!=( const HValue& rhs ) const;
		bool				operator<( const HValue& rhs ) const;

	private:
		IValue*				m_value;
	};
}

#endif