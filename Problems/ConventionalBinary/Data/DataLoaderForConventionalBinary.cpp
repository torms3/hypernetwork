//
// Copyright 2011 by 
//		kslee@bi.snu.ac.kr
//		hmlee@bi.snu.ac.kr
//

#include "DataLoaderForConventionalBinary.h"
#include "PrimitiveValue.h"
#include <fstream>
#include <sstream>

using std::string;
using std::ifstream;
using std::getline;
using std::istringstream;

#if defined( _TEST )
#include <iostream>
using std::cout;
using std::endl;
#endif

namespace HN
{
	//////////////////////////////////////////////////////////////////////////
	// Implementation for ConvBinLoader
	//
	ConvBinLoader::ConvBinLoader( string fileName )
	{
		SetInputFileName( fileName );
	}

	ConvBinLoader::~ConvBinLoader()
	{
	}

	void ConvBinLoader::LoadData( DataSet* dataSet )
	{
		// DataSet should exist
		if( !dataSet )
			return;

		// Input file should exist
		ifstream fin( m_inputFileName );
		if( !fin )
			return;

		// File metadata
		int rows = 0;
		int cols = 0;
		string lastLine;

		// First pass parsing
		{
			// The first pass parsing to figure out # of rows
			string line;			
			while( getline( fin, line ) )
			{				
				lastLine = line;				
				rows++;
			}

			// Parsing the last line to figure out # of cols
			istringstream sin( lastLine );
			unsigned int val = 0;			
			while( sin >> val ) cols++;
		}

		// Close and re-open for the second pass
		fin.close();
		fin.open( m_inputFileName );

		// Second pass parsing
		{
			// Add data samples to DataSet
			for( int i = 0; i < cols; ++i )
			{
				Data tempData;
				dataSet->AddData( tempData );
				dataSet->GetData( i )->SetDimension( rows - 1 );
			}

			string line;
			// Preserve the last line for label processing
			for( int i = 0; i < rows - 1; ++i )
			{
				if( getline( fin, line ) )
				{
					istringstream sin( line );
					if( sin )
					{
#if defined( _TEST )
						//cout << "row: " << i << endl;
#endif

						for( int j = 0; j < cols; ++j )
						{
							unsigned short val = 0;
							if( sin >> val )
							{
								PrimitiveValue<bool> tempVal( (val ? true : false) );
								HValue tempHandle( &tempVal );
								Node tempNode( i, tempHandle );
								dataSet->GetData( j )->SetNode( i, tempNode );

								continue;
							}
							
							throw std::runtime_error( "an error occurs during input file parsing" );
						}

						continue;
					}
				}
				
				throw std::runtime_error( "an error occurs during input file parsing" );
			}

			// Label processing
			istringstream sin( lastLine );
			if( !sin ) throw std::runtime_error( "an error occurs during input file parsing" );
			for( int i = 0; i < cols; ++i )
			{
				unsigned short val = 0;
				if( sin >> val )
				{
					Label tempLabel;
					tempLabel.SetValue( val );
					dataSet->GetData( i )->SetLabel( tempLabel );

					continue;
				}

				throw std::runtime_error( "an error occurs during input file parsing" );
			}
		}

		// Close the file stream
		fin.close();
	}

	void ConvBinLoader::SetInputFileName( string fileName )
	{
		m_inputFileName = fileName;
	}
}