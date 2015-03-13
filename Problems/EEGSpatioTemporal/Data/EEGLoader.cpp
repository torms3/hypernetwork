//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "EEGLoader.h"
#include "DataForEEGSpatioTemporal.h"
#include "PrimitiveValue.h"
#include <boost/random.hpp>
#include <ctime>

#include <fstream>
using std::ifstream;

namespace HN
{
	EEGLoader::EEGLoader( )
	{
		
	}

	EEGLoader::~EEGLoader()
	{
	}

	void EEGLoader::LoadData( DataSet* dataSet )
	{
		if( !dataSet )
			return;

#if defined( _TEST )
		ifstream fin( "jskim_m1_dis_trans.txt" );
#else
		ifstream fin( "EEG_Input.txt" );
#endif
		
		int signal_size, time_size;
		fin >> signal_size >> time_size;

		// There's an only one data for Basic Spatio-Temporal Analysis.
		{
			EEGData tempData(signal_size, time_size);
			// [04/27/2011 kslee]
			// For optimization
			tempData.SetDimension( signal_size * time_size );

			for( int i = 0; i < signal_size; ++i )
			{
				for( int j = 0; j < time_size; ++j )
				{
					int val;
					fin >> val;
					PrimitiveValue<int> tempVal( val );
					HValue tempHandle( &tempVal );
					Node tempNode( tempData.GetIndex(i, j), tempHandle );
					// [04/27/2011 kslee]
					// For optimization
					tempData.SetNode( tempNode.GetIndex(), tempNode );
				}
			}

			dataSet->AddData( tempData );
		}

		fin.close();
	}
}