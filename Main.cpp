//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "HyperNetwork.h"
#include "DataSet.h"
#include "TestLoader.h"
#include "ConventionalBinary.h"
#include "DataSetForConventionalBinary.h"
#include "DataLoaderForConventionalBinary.h"
#include "EEGLoader.h"
#include "EEGSpatioTemporalLM.h"

#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;

using namespace HN;

enum {
	HN_ConventionalBinary = 0,
	HN_EEGSpatioTemporal  = 1,
};

bool ConventionalBinary_Main();
bool EEGSpatioTemporal_Main();

int main()
{	
	cout << "Select the Hypernetwork." << endl;
	cout << "0. Conventional Binary" << endl;
	cout << "1. EEG Spatio-Temporal Analysis" << endl;

	int HNcase;
	cin >> HNcase;
	switch( HNcase ) 
	{
	case HN_ConventionalBinary:				
		ConventionalBinary_Main();				
		break;
	case HN_EEGSpatioTemporal:
		EEGSpatioTemporal_Main();
		break;
	}
	
	return 0;
}

bool ConventionalBinary_Main()
{
	HyperNetwork* hn = new HyperNetwork();
	DataSet* ds1 = new ConvBinDataSet();
	DataSet* ds2 = new ConvBinDataSet();

	IDataLoader* dataLoader = new TestConvBinLoader( 5, 5, 10, 10 );
	dataLoader->LoadData( ds1 );
		
	ofstream fout( "profile.txt" );
	clock_t before, after;

	IDataLoader* convBinLoader = new ConvBinLoader( "ConvBin_Input.txt" );
	before = clock();
	//convBinLoader->LoadData( ds2 );
	after = clock();
	fout << "ConvBinLoader::LoadData(): " <<  (after - before) / CLOCKS_PER_SEC << " secs" << endl;

	ILearningMachine* convBin = new ConvBinLM();	
	convBin->SetTrainingSet( HDataSet( ds1 ) );
	//convBin->SetTrainingSet( HDataSet( ds2 ) );

	convBin->Training();

	delete convBin;

	delete convBinLoader;
	delete dataLoader;

	before = clock();
	// [04/21/2011 kslee]
	// There is a severe non-linear temporal behavior during delete.
	// Temporarily, delete process is blocked.
	//delete ds2;
	after = clock();
	fout << "DataSet::~DataSet(): " <<  (after - before) / CLOCKS_PER_SEC << " secs" << endl;
	delete ds1;	
	
	delete hn;

	return true;
}

bool EEGSpatioTemporal_Main()
{
	HyperNetwork* hn = new HyperNetwork();
	DataSet* ds = new DataSet();

	IDataLoader* dataLoader = new EEGLoader( );
	dataLoader->LoadData( ds );

	//vector<HData> trainingSet;
	//for( int i = 0; i < ds->GetNumData(); ++i )
	//{
	//	trainingSet.push_back( ds->GetData( i ) );
	//}
	
	ILearningMachine* eegSpatioTemporalLM = new EEGSpatioTemporalLM();
	HDataSet trainingSet( ds );
	eegSpatioTemporalLM->SetTrainingSet( trainingSet );

	eegSpatioTemporalLM->Training();
	
	delete dataLoader;
	delete ds;

	delete hn;

	return true;

	return true;
}