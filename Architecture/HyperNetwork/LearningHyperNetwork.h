#ifndef _LEARNING_HYPERNETWORK_H_
#define _LEARNING_HYPERNETWORK_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "HyperNetwork.h"
#include "Data.h"

namespace HN
{	
	//////////////////////////////////////////////////////////////////////////
	// Learning HyperNetwork
	//
	class LearningHN : public HyperNetwork
	{
	public:
		// Operations for edges
		void				RemoveLastEdge();
		void				MatchEdge( HData hData );
		void				ScoreEdge();
		void				SortEdge();
		void				SeparateEdgesByClass( map<Label, vector<HEdge> >& edgesByClass );
	};
}

#endif
