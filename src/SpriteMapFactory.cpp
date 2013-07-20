#include "SpriteMapFactory.h"
#include "Utilities.h"
#include "Log.h"
#include <fstream>

using namespace std;
using namespace Escarabajo;


SpriteMap SpriteMapFactory::load( string filename )
{
    SpriteMapParser parser;
    parser.load( filename );
    return parser.spriteMap;
}


void SpriteMapFactory::SpriteMapParser::section( std::string sectionName )
{
    // Nothing to do?
}


void SpriteMapFactory::SpriteMapParser::pair( std::string name, std::string value )
{
     if ( name.length() != 1 )
     {
        Log::write( "Whoa there cowboy, all sprite symbols must be a single character" );
        Log::write( "    Problematic symbol: " + name );
        return;
     }

     // Load sprite.
     Sprite sprite;
     sprite.loadImageSequence( Utilities::stringToVector( value, ',' ) );

     // Add to the map.
     spriteMap[ name[0] ] = sprite;
}

