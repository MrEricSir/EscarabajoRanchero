#include "LevelFileReader.h"
#include "Log.h"

using namespace std;
using namespace Escarabajo;

LevelFileReader::LevelFileReader()
{
    // Do NOTHING!!!
}

LevelFileReader::LevelFileReader( std::string filename )
{
    load( filename );
}


// Loads a file.
bool LevelFileReader::load( std::string filename )
{
    // Unload previous file.
    sections.clear();

    // Parse the file.
    string line = "";
    string mine = "";
    string currentSection = "";
    fstream file( filename.c_str() );

    // Read through file.
    while ( file.good() )
    {
        getline( file, line );
        stringstream s( line );
        string mine;
        s >> mine;

        // Figure out what type of line we have.
        if ( mine.length() == 0 )
        {
            // No data, do nothing.
        }
        else if ( mine[0] == '#' )
        {
            // Comment, also do nothing.
        }
        else if ( mine[0] == '[' )
        {
            // Section header.
            currentSection = mine.substr( 1, mine.length() - 2 );
            Log::write( "Current section: " + currentSection );
        }
        else
        {
            // Otherwise it's just a plain ol' line and
            // we gotta save that yo.
            sections[ currentSection ].push_back( line );
        }
    }

    // Close our file.
    file.close();

    return true;
}

// Checks if a section exists.
bool LevelFileReader::hasSection( std::string sectionName )
{
    map<string, vector<string> >::const_iterator it = sections.find( sectionName );
    return ( it != sections.end() );
}


// Gets a section as a string vector.
std::vector<std::string> LevelFileReader::getSection( std::string sectionName )
{
    return sections[ sectionName ];
}

