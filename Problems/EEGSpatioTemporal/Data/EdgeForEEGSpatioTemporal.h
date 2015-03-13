#ifndef _EDGEFOREEGSPATIOTEMPORAL_H_
#define _EDGEFOREEGSPATIOTEMPORAL_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "EdgeForLearning.h"
#include <fstream>
using namespace std;

namespace HN
{
	//
	// Edge for EEG Spatio-Temporal problem
	//
	class EEGEdge : public LearningEdge
	{
	public:
							EEGEdge();
		virtual				~EEGEdge();

		virtual void		Match( HData hData );

		void				AddIndex(int t, int edgeIndex);
		int					GetTimeSize() const					{ return m_indexTable.size(); }
		vector< int >&		GetNodeIndexList( int t );

		// [04/29/2011 hmlee]
		// [Temp] Get signal_size, time_size from Learning Machine
		// Refactoring later.
		void				Print(ofstream& fout, int matched_count, int cnt, int signal_size, int time_size);
		
#if defined( _TEST )
		virtual string		TestString( string format ) const;
#endif

	protected:
		virtual bool		EqualsTo( const Edge& e ) const;
		virtual bool		LessThan( const Edge& e ) const;
		virtual Edge*		Clone() const						{ return new EEGEdge( *this ); }

	private:
		vector< vector< int > > m_indexTable;
	};
}

#endif