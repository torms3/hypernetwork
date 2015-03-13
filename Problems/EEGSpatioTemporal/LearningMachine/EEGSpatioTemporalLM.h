#ifndef _EEGSPATIOTEMPORALLM_H_
#define _EEGSPATIOTEMPORALLM_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "LearningMachine.h"
#include "DataForEEGSpatioTemporal.h"
#include "EdgeForEEGSpatioTemporal.h"
#include <boost/random.hpp>

#if defined( _TEST )
#include <fstream>
using std::ofstream;
#endif

namespace HN
{
	class EEGSpatioTemporalLM : public ILearningMachine
	{
	public:
							EEGSpatioTemporalLM();
							~EEGSpatioTemporalLM();

		virtual void		Training();

	protected:
		virtual void		Initialize();
		virtual void		Sampling();
		void				Merging();
		virtual void		Scoring();
		virtual void		Pruning();
		virtual void		Matching();
		virtual bool		IsEnough();
		virtual void		Finalize();

	private:
		// Sampling
		int					GetSamplingSignalOrder( int classLabel );
		int					GetSamplingTimeOrder( int classLabel );
		// Merging
		bool				CompareTwoEdge( EEGEdge* e0, EEGEdge* e1 );
		EEGEdge				MergeTwoEdge( EEGEdge* e0, EEGEdge* e1 );
		// Scoring
		void				CalcLibraryScore();
		// Pruning
		void				EliminateBadEdge();
		// Match
		int					Matched( EEGEdge* edge, EEGData* data );
		// Random
		int					GetRandomValue(boost::mt19937& rng, int limit);
		vector< int >		GetRandomValueList(boost::mt19937& rng, int limit, int number);

		double				GetMutationRatio( int iter );

		int					GetSignalIndex( int index )				{ return index / m_time_size; }
		int					GetTimeIndex( int index )				{ return index % m_time_size; }

		int					m_signal_size;
		int					m_time_size;

		ofstream			m_fout;

		// Finalize
#if defined( _TEST )
		void				TestFilePrint() const;
#endif

	private:
		static const string		CONFIG_FILE_NAME;
		static const string		OUTPUT_FILE_NAME;

#if defined( _TEST )
		ofstream			m_fout2;
#endif
	};
}

#endif