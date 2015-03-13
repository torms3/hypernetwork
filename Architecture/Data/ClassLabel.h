#ifndef _CLASS_LABEL_H_
#define _CLASS_LABEL_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "Value.h"

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Represents class label Y.
	//
	class Label : public IValue
	{
	public:				
							 Label(){}
		virtual				~Label(){}

		bool				operator==( const Label& rhs ) const;
		bool				operator!=( const Label& rhs ) const;
		bool				operator<( const Label& rhs ) const;
				
		int					GetValue() const	{ return m_label; }
		void				SetValue( int v )	{ m_label = v; }

#if defined( _TEST )
		string				TestString() const;
#endif

	protected:
		virtual IValue*		Clone() const		{ return new Label( *this ); }
		virtual bool		EqualsTo( const IValue& val ) const;
		virtual bool		LessThan( const IValue& val ) const;

	private:
		int					m_label;
	};
}

#endif
