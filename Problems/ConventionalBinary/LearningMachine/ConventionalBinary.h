#ifndef _CONVENTIONAL_BINARY_H_
#define _CONVENTIONAL_BINARY_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "LearningMachine.h"
#include <boost/random.hpp>

#include <map>
using std::map;

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Learning machine for Conventional Binary problems
	//
	class ConvBinLM : public ILearningMachine
	{
	public:
								ConvBinLM();
		virtual					~ConvBinLM();

	protected:
		virtual void			Initialize();
		virtual void			Sampling();
		virtual void			Matching();
		virtual void			Scoring();
		virtual void			Pruning();
		virtual bool			IsEnough();
		virtual void			Finalize();	

	private:
		// Sampling
		void					InitOrderDistribution();
		void					CountOrder();
		int						GetSamplingOrder( Label label );
		void					Sampling( int dataIndex );

		// Scoring
		void					CalcLibraryScore();

		// Pruning
		void					EliminateBadEdgeByClass();
		double					GetMutationRatio( int iter );

		// Finalize
#if defined( _TEST )
		void					TestFilePrint() const;
#endif
	
	private:
		typedef map<int, int> OrderDist;

	private:
		vector< int >			m_dataIndexForRemovedEdge;
		boost::mt19937			m_rng;

		// Order
		map<Label, int>			m_orderSumByClass;
		map<Label, OrderDist>	m_orderDistByClass;

	private:
		static const string		CONFIG_FILE_NAME;
	};
}

#endif