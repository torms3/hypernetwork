//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "TestLoader.h"
#include "PrimitiveValue.h"
#include <ctime>

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for TestConvBinLoader
	//
	TestConvBinLoader::TestConvBinLoader( int minNumData/* = MIN_NUM_DATA*/, 
										  int maxNumData/* = MAX_NUM_DATA*/,
										  int minDimension/* = MIN_DIMENSION*/,
										  int maxDimension/* = MAX_DIMENSION*/ )
	{
		if( minNumData > maxNumData )
			minNumData = maxNumData;

		if( minDimension > maxDimension )
			minDimension = maxDimension;

		m_minNumData = (minNumData < MIN_NUM_DATA ? MIN_NUM_DATA : minNumData);
		m_maxNumData = (maxNumData > MAX_NUM_DATA ? MAX_NUM_DATA : maxNumData);
		m_minDimension = (minDimension < MIN_DIMENSION ? MIN_DIMENSION : minDimension);
		m_maxDimension = (maxDimension > MAX_DIMENSION ? MAX_DIMENSION : maxDimension);

		m_rng.seed( static_cast<unsigned int>( time( 0 ) ) );
	}

	TestConvBinLoader::~TestConvBinLoader()
	{
	}

	void TestConvBinLoader::LoadData( DataSet* dataSet )
	{
		if( !dataSet )
			return;

		// Determines the number of data
		boost::uniform_int<> numDataDistribution( m_minNumData, m_maxNumData );
		boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dieForNumData( m_rng, numDataDistribution );
		int numData = dieForNumData();

		// Determines the dimension
		boost::uniform_int<> dimensionDistribution( m_minDimension, m_maxDimension );
		boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dieForDimension( m_rng, dimensionDistribution );
		int dimension = dieForDimension();

		// For random bool value generation
		boost::uniform_int<> boolDistribution( 0, 1 );
		boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die( m_rng, boolDistribution );

		// Generate random data set
		for( int i = 0; i < numData; ++i )
		{
			Data tempData;

			for( int j = 0; j < dimension; ++j )
			{
				PrimitiveValue<bool> tempVal( (die() ? true : false) );
				HValue tempHandle( &tempVal );
				Node tempNode( j, tempHandle );
				tempData.AddNode( tempNode );
			}

			Label label;
			label.SetValue( die() );
			tempData.SetLabel( label );

			dataSet->AddData( tempData );
		}
	}
}