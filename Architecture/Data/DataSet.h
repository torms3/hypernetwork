#ifndef _DATA_SET_H_
#define _DATA_SET_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "Data.h"

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Data Set D = {X1, X2, X3, .... }	
	//
	class DataSet
	{
	public:
		template< typename T >
		friend T*			Clone( const T* data );

	public:
							DataSet();
		virtual				~DataSet(){}

		HData				GetData( int index )	{ return m_data.at( index ); }
		virtual void		AddData( Data& data );
		int					GetNumData() const		{ return m_data.size(); }
	
		int					GetDimension() const	{ return m_dimension; }

		void				Shuffle();

	protected:
		virtual DataSet*	Clone() const			{ return new DataSet( *this ); }

	private:
		vector< HData >		m_data;
		int					m_dimension;
	};

	//////////////////////////////////////////////////////////////////////////
	// Reference handle for DataSet class
	//
	typedef RefHandle< DataSet >	HDataSet;
}

#endif