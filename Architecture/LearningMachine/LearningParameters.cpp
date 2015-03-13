//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "LearningParameters.h"

#include <fstream>
using std::ifstream;
#include <sstream>
using std::stringstream;
using std::istringstream;
#include <stdexcept>

// [04/23/2011 kslee]
// Disable warning for string::copy()
#pragma warning( disable:4996 )

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for LearningParameters class
	//
	LearningParameters::LearningParameters()	
	{
	}

	LearningParameters::~LearningParameters()
	{
	}

	void LearningParameters::Load( string fileName )
	{
		Clear();
		Initialize( fileName );
	}

	int LearningParameters::GetIntParam( string name ) const
	{
		map<string, int>::const_iterator it = m_iParam.find( name );
		if( it != m_iParam.end() )
		{
			return it->second;
		}
		throw std::runtime_error( "non-existing parameter" );
	}

	double LearningParameters::GetDoubleParam( string name ) const
	{
		map<string, double>::const_iterator it = m_dParam.find( name );
		if( it != m_dParam.end() )
		{
			return it->second;
		}
		throw std::runtime_error( "non-existing parameter" );
	}

	void LearningParameters::Clear()
	{
		m_iParam.clear();
		m_dParam.clear();
	}

	void LearningParameters::Initialize( string fileName )
	{
		ifstream configFile( fileName );
		if( configFile )
		{
			stringstream buffer;
			buffer << configFile.rdbuf();
			
			string s_xml = buffer.str();
			char* ch_xml = new char[s_xml.length() + 1];
			memset( ch_xml, 0, s_xml.length() + 1 );
			s_xml.copy( ch_xml, s_xml.length() );

			xml_document<> config;
			config.parse<0>( ch_xml );

			ParseXML( config );
			
			config.clear();
			delete[] ch_xml;
		}
		configFile.close();
	}

	void LearningParameters::ParseXML( xml_document<>& config )
	{
		xml_node<>* root = config.first_node( "parameters" );
		if( !root )
			return;
		
		xml_node<>* node = root->first_node( "param" );
		for( ; node; node = node->next_sibling( "param" ) )
		{
			xml_attribute<>* name = node->first_attribute( "name" );
			xml_attribute<>* type = node->first_attribute( "type" );			
			if( !name || !type )
			{
				// Simply ignore invalid parameter
				continue;
			}
			string s_name = name->value();
			string s_type = type->value();
			
			xml_node<>* val = node->first_node();
			if( !val )
			{
				// Simply ignore invalid parameter
				continue;
			}
			string s_val = val->value();
			
			istringstream sin( s_val );
			if( !sin )
			{
				// Simply ignore invalid parameter
				continue;
			}

			if( s_type == "int" )
			{
				int val;
				if( sin >> val )
				{
					m_iParam[s_name] = val;
				}
			}
			else if( s_type == "double" )
			{
				double val;
				// [04/23/2011 kslee]
				// There exists some kind of precision issue.
				// That is,
				// when read the value from string "0.6", 
				// it reads 0.59999999999999998.
				// It is not certain whether this will cause some problems.
				if( sin >> val )
				{
					m_dParam[s_name] = val;
				}
			}
			else
			{
				// Simply ignore the attribute of undefined type
				continue;
			}
		}
	}
}