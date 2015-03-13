#ifndef _LEARNING_PARAMETERS_H_
#define _LEARNING_PARAMETERS_H_

//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "rapidxml.hpp"
using namespace rapidxml;

#include <map>
using std::map;
#include <string>
using std::string;

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Class for managing experimental learning parameters.
	// Using XML parsing.
	//
	class LearningParameters
	{
	public:
								LearningParameters();
								~LearningParameters();

		void					Load( string fileName );

		int						GetIntParam( string name ) const;
		double					GetDoubleParam( string name ) const;

	private:
		void					Clear();
		void					Initialize( string fileName );		
		void					ParseXML( xml_document<>& config );

	private:
		map<string, int>		m_iParam;
		map<string, double>		m_dParam;
	};
}

#endif