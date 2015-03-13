//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "DataForEEGSpatioTemporal.h"
#include <algorithm>

namespace HN
{	
	//
	// Implementation for Data class
	//
	EEGData::EEGData(int signal_size, int time_size)
	{
		m_signal_size = signal_size;
		m_time_size = time_size;
	}

	EEGData::~EEGData()
	{
	}

}