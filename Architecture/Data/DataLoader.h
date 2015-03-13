#ifndef _DATA_LOADER_H_
#define _DATA_LOADER_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "DataSet.h"

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Loads data from files and constructs data set for further use.
	//
	class IDataLoader
	{
	public:
						IDataLoader(){}
		virtual			~IDataLoader(){}

		virtual void	LoadData( DataSet* dataSet ) = 0;
	};
}

#endif