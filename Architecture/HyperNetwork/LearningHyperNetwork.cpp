//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "LearningHyperNetwork.h"
#include "EdgeForLearning.h"
#include <algorithm>

namespace HN
{	
	//////////////////////////////////////////////////////////////////////////
	// Implementation for LearningHN class
	//
	void LearningHN::RemoveLastEdge()
	{	
		//HEdge hEdge = m_edge.back();
		//HWeight hWeight = hEdge->GetWeight();
		//RemoveWeight( hWeight );

		m_edge.pop_back();
	}

	void LearningHN::MatchEdge( HData hData )
	{
		vector<HEdge>::iterator it = m_edge.begin();
		for( ; it != m_edge.end(); ++it )
		{
			LearningEdge* edgeL = dynamic_cast<LearningEdge*>( (*it).GetDataPtr() );
			if( edgeL )
			{
				edgeL->Match( hData );
			}
		}
	}

	void LearningHN::ScoreEdge()
	{
		vector<HEdge>::iterator it = m_edge.begin();
		for( ; it != m_edge.end(); ++it )
		{
			LearningEdge* edgeL = dynamic_cast<LearningEdge*>( (*it).GetDataPtr() );
			if( edgeL )
			{
				edgeL->Score();
			}
		}
	}

	void LearningHN::SortEdge()
	{
		std::sort( m_edge.rbegin(), m_edge.rend() );
	}

	void LearningHN::SeparateEdgesByClass( map<Label, vector<HEdge> >& edgesByClass )
	{
		vector<HEdge>::iterator it = m_edge.begin();
		for( ; it != m_edge.end(); ++it )
		{
			edgesByClass[(*it)->GetLabel()].push_back( *it );
		}
	}
}