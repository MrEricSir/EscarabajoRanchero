#ifndef LEVELFILEREADER_H_
#define LEVELFILEREADER_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

namespace Escarabajo
{

/**
 * Loads data from a level file into a string table.
 */
class LevelFileReader
{
public:

	LevelFileReader();

	// Loads a file.
	LevelFileReader( std::string filename );

	// Loads a file.
	virtual bool load( std::string filename );

	// Checks if a section exists.
	bool hasSection( std::string sectionName );

	// Gets a section as a string vector.
	virtual std::vector<std::string> getSection( std::string sectionName );

protected:

	// Section map.
	std::map<std::string, std::vector<std::string> > sections;

};

}

#endif /* LEVELFILEREADER_H_ */
