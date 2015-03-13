#ifndef _DATA_SET_FOR_CONVENTIONAL_BINARY_H_
#define _DATA_SET_FOR_CONVENTIONAL_BINARY_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "DataSet.h"
#include <vector>
#include <map>

using std::vector;
using std::map;

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Data Set for Conventional Binary
	//
	class ConvBinDataSet : public DataSet
	{
	public:
							ConvBinDataSet();
		virtual				~ConvBinDataSet(){}

		// Basic DataSet operations
		virtual void		AddData( Data& data );

		// Operations specific for ConvBinDataSet
		int					GetClassCount( Label& label);
		void				GetLabelSet( vector<Label>& labelSet ) const;

	protected:
		virtual DataSet*	Clone() const	{ return new ConvBinDataSet( *this ); }

	private:
		map< Label, int >	m_classCount;
	};
}

#endif