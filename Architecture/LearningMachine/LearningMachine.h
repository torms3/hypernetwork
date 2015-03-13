#ifndef _LEARNINGMACHINE_H_
#define _LEARNINGMACHINE_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "LearningHyperNetwork.h"
#include "DataSet.h"
#include "LearningParameters.h"

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Learning Machine
	//
	class ILearningMachine
	{
	public:
							ILearningMachine();
		virtual				~ILearningMachine();

		virtual void		Training();

		void				SetTrainingSet( HDataSet& dataSet );
		void				SetTestSet( HDataSet& dataSet );

		bool				IsTraining() const	{ return m_isTraining; }
		
	protected:
		virtual void		Initialize()		= 0;
		virtual void		Sampling()			= 0;
		virtual void		Matching()			= 0;
		virtual void		Scoring()			= 0;
		virtual void		Pruning()			= 0;
		virtual	bool		IsEnough()			= 0;
		virtual void		Finalize()			= 0;

	protected:
		LearningHN			m_library;

		HDataSet			m_trainingSet;
		HDataSet			m_testSet;

		LearningParameters	m_parameters;

		int					m_iteration;
		bool				m_isTraining;

	private:
	};
}

#endif