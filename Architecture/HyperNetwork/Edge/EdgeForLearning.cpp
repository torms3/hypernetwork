//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "EdgeForLearning.h"

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for LearningEdge class
	//
	void LearningEdge::Match( HData hData )
	{
		Update( IsMatched( hData ), hData );
	}

	void LearningEdge::Score()
	{

	}

	bool LearningEdge::IsMatched( HData hData )
	{
		vector<HNode>::iterator it = m_node.begin();
		for( ; it != m_node.end(); ++it )
		{
			Node& lhs = *(*it);
			Node& rhs = hData->GetNode( lhs.GetIndex() );
			if( lhs != rhs )
			{
				return false;
			}
		}

		return true;
	}

	void LearningEdge::Update( bool match, HData hData )
	{
		if( match )
		{
			m_hWeight->IncWeight( 1 );
		}
		else
		{
			m_hWeight->DecWeight( 1 );
		}
	}
}