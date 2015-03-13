#ifndef _TEST_LOADER_H_
#define _TEST_LOADER_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "DataLoader.h"
#include <boost/random.hpp>

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Test DataLoader for Conventional Binary problem.
	// Generate DataSet randomly.
	//
	class TestConvBinLoader : public IDataLoader
	{
	public:
							TestConvBinLoader( int minNumData = MIN_NUM_DATA, 
											   int maxNumData = MAX_NUM_DATA,
											   int minDimension = MIN_DIMENSION,
											   int maxDimension = MAX_DIMENSION );
		virtual				~TestConvBinLoader();

		virtual void		LoadData( DataSet* dataSet );

	private:
		int					m_minNumData;
		int					m_maxNumData;
		int					m_minDimension;
		int					m_maxDimension;

		boost::mt19937		m_rng;

	private:
		static const int	MIN_NUM_DATA		= 1;
		static const int	MAX_NUM_DATA		= 62;		// Same as the original one
		static const int	MIN_DIMENSION		= 1;
		static const int	MAX_DIMENSION		= 81924;	// Same as the original one
	};
}

#endif