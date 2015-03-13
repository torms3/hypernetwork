#ifndef _EDGE_FOR_CONVENTIONAL_BINARY_H_
#define _EDGE_FOR_CONVENTIONAL_BINARY_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "EdgeForLearning.h"
#include "DataSet.h"
#include <map>

using std::map;

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Edge for conventional binary problem
	//
	class ConvBinEdge : public LearningEdge
	{
	public:
							ConvBinEdge(){}
		virtual				~ConvBinEdge(){}

		// Operations for learning
		virtual void		Match( HData hData );
		virtual void		Score();

		// Operations specialized for Conventional Binary
		int					GetDataIndex() const				{ return m_data.dataIndex; }
		void				SetDataIndex( int index )			{ m_data.dataIndex = index; }
		bool				IsNew() const						{ return m_data.bNew; }
		void				SetNew( bool b )					{ m_data.bNew = b; }
		void				SetTrainingSet( HDataSet& dataSet)	{ m_trainingSet = dataSet; }
		
#if defined( _TEST )
		virtual string		TestString( string format ) const;
#endif

	protected:
		virtual Edge*		Clone() const						{ return new ConvBinEdge( *this ); }

		// Operations for learning
		virtual void		Update( bool match, HData hData );
		
	private:
		struct Data
		{
			int				dataIndex;
			bool			bNew;
			map<Label, int>	classCount;
			double			score;

#if defined( _TEST )
			string			TestString( string format ) const;
#endif
		};

	private:
		ConvBinEdge::Data	m_data;
		HDataSet			m_trainingSet;

	private:
		static const int	ALPHA	= 200;
		static const int	BETA	= 1;
		static const int	GAMMA	= 50;
	};
}

#endif