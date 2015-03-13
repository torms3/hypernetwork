//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "DataSetForConventionalBinary.h"

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for ConvBinDataSet class
	//
	ConvBinDataSet::ConvBinDataSet()
	{
		m_classCount.clear();
	}

	void ConvBinDataSet::AddData( Data& data )
	{
		DataSet::AddData( data );

		m_classCount[data.GetLabel()]++;
	}

	int ConvBinDataSet::GetClassCount( Label& label )
	{
		return m_classCount[label];
	}

	void ConvBinDataSet::GetLabelSet( vector<Label>& labelSet ) const
	{
		labelSet.clear();

		map<Label, int>::const_iterator it = m_classCount.begin();
		while( it != m_classCount.end() )
		{
			labelSet.push_back( it->first );
			++it;
		}
	}
}