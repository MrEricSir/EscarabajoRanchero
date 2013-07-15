
#ifndef LEVELFILEREADER_PNG_H_
#define LEVELFILEREADER_PNG_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include "LevelFileReader.h"


namespace Escarabajo
{

/**
 * Loads data from a PNG level file into a string table.
 */
class LevelFileReaderPng : public LevelFileReader
{
public:

	LevelFileReaderPng();

		// Loads a file.
	LevelFileReaderPng( std::string filename );

	// Loads a file.
	bool load( std::string filename );


};

}

#endif /* LEVELFILEREADER_PNG_H_ */
