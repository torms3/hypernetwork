//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "LearningMachine.h"

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for LearningMachine class
	//
	ILearningMachine::ILearningMachine()	
		: m_isTraining( false )
		, m_iteration( 0 )
	{
	}

	ILearningMachine::~ILearningMachine()
	{
	}

	void ILearningMachine::SetTrainingSet( HDataSet& dataSet )
	{
		if( !IsTraining() )
		{
			m_trainingSet = dataSet;
		}
	}

	void ILearningMachine::SetTestSet( HDataSet& dataSet )
	{
		if( !IsTraining() )
		{
			m_testSet = dataSet;
		}
	}

	void ILearningMachine::Training()
	{
		m_isTraining = true;
		m_iteration = 0;

		Initialize();

		// Training loop
		do
		{
			m_iteration++;

			Sampling();
			Matching();
			Scoring();
			Pruning();

		} while( !IsEnough() );

		Finalize();

		m_isTraining = false;
	}
}