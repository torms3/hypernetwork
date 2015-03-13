#ifndef _EDGE_FOR_LEARNING_H_
#define _EDGE_FOR_LEARNING_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "Edge.h"
#include "Data.h"

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Edge for learning
	//
	class LearningEdge : public Edge
	{
	public:
							LearningEdge(){}
		virtual				~LearningEdge(){}

		// Operations for learning
		virtual void		Match( HData hData );
		virtual void		Score();

	protected:
		virtual Edge*		Clone() const	{ return new LearningEdge( *this ); }

		// Operations for learning
		virtual bool		IsMatched( HData hData );
		virtual void		Update( bool match, HData hData );
	};
}

#endif