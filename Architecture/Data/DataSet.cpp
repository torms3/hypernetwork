//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "DataSet.h"
#include <algorithm>

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for DataSet class
	//
	DataSet::DataSet()
		: m_dimension( 0 )
	{
	}

	void DataSet::AddData( Data& data )
	{
		m_data.push_back( HData( &data ) );
		m_dimension = std::max( m_dimension, data.GetDimension() );
	}

	void DataSet::Shuffle()
	{
		std::random_shuffle( m_data.begin(), m_data.end() );
	}
}