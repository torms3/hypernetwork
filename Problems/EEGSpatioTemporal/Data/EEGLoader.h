#ifndef _EEGLOADER_H_
#define _EEGLOADER_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "DataLoader.h"

namespace HN
{
	//
	// DataLoader for EEG Data.
	//
	class EEGLoader : public IDataLoader
	{
	public:
							EEGLoader( );
		virtual				~EEGLoader();

		virtual void		LoadData( DataSet* dataSet );

	private:

	};
}

#endif