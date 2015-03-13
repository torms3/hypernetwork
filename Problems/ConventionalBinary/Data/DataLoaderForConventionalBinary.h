#ifndef _DATA_LOADER_FOR_CONVENTIONAL_BINARY_H_
#define _DATA_LOADER_FOR_CONVENTIONAL_BINARY_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "DataLoader.h"
#include <string>

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// DataLoader for Conventional Binary problem.	
	//
	class ConvBinLoader : public IDataLoader
	{
	public:
							ConvBinLoader( std::string fileName );
		virtual				~ConvBinLoader();

		virtual void		LoadData( DataSet* dataSet );

		void				SetInputFileName( std::string fileName );

	private:
		std::string			m_inputFileName;
	};
}

#endif