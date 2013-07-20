#include "MapFileReader.h"
#include "Log.h"

using namespace std;
using namespace Escarabajo;

bool MapFileReader::load( string filename )
{
    Log::write( "Loading file " + filename );
    string line;
    fstream file( filename.c_str() );
    if ( !file || file.bad() ) return false;

    // Read through file.
    while ( file.good() )
    {
        getline( file, line );

        // No data.
        if ( line.length() == 0 ) continue;

        // Comment.
        if ( line[0] == '#' ) continue;

        // Section header.
        if ( line[0] == '[' && line[line.size() - 1] == ']' )
        {
            section( line.substr( 1, line.size() -2 ) );
            continue;
        }

        // Pair.
        if ( Utilities::contains( line, '=' ) )
        {
            // Find the = sign.
            size_t equals = line.find( '=' );


            // Get name.
            string name = Utilities::removeExtraSpaces( line.substr( 0, equals ) );
            if ( name.length() == 0 )
            {
                // Empty line.
                continue;
            }

            // Get value.
            string value = Utilities::removeExtraSpaces( line.substr( equals + 1 ) );

            pair( name, value );
        }
    }

    // Close our file.
    file.close();

    return true;
}
