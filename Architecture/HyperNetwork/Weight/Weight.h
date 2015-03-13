#ifndef _WEIGHT_H_
#define _WEIGHT_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "RefHandle.h"

namespace HN
{	
	//////////////////////////////////////////////////////////////////////////
	// Weight W = { w_1, w_2, ..., w_|E| }
	//
	class Weight
	{
	public:
		template< typename T >
		friend T*			Clone( const T* data );

	public:
							Weight( int weight = 0 ) : m_weight( weight ) {}
		virtual				~Weight(){}

		bool				operator==( const Weight& rhs ) const	{ return EqualsTo( rhs ); }
		bool				operator!=( const Weight& rhs ) const	{ return !EqualsTo( rhs ); }
		bool				operator<( const Weight& rhs ) const	{ return LessThan( rhs ); }

		int					GetWeight() const						{ return m_weight; }
		void				SetWeight( int weight )					{ m_weight = weight; }

		void				IncWeight( int amount )					{ m_weight += amount; }
		void				DecWeight( int amount )					{ m_weight -= amount; }

	protected:
		virtual	Weight*		Clone() const							{ return new Weight( *this ); }
		virtual bool		EqualsTo( const Weight& w ) const		{ return (m_weight == w.m_weight); }
		virtual bool		LessThan( const Weight& w ) const		{ return (m_weight < w.m_weight); }

	private:
		int					m_weight;
	};

	//////////////////////////////////////////////////////////////////////////
	// Handle for Node Weight
	//
	typedef RefHandle< Weight >	HWeight;
}

#endif