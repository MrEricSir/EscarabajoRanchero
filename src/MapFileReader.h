#ifndef MAPFILEREADER_H_
#define MAPFILEREADER_H_

#include "Utilities.h"
#include <fstream>
#include <iostream>

namespace Escarabajo
{

/**
 * This class is intended to be extended to read
 * in a map file.
 */
class MapFileReader
{
public:

	// Loads a map file.  Returns true on success.
	bool load( std::string filename );

	// Override this to get a [section] header.
	virtual void section( std::string sectionName ){}

	// Override this to get a name = value pair.
	virtual void pair( std::string name, std::string value ){}

};

}


#endif /* MAPFILEREADER_H_ */
