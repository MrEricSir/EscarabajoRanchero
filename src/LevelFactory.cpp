#include "LevelFactory.h"
#include "SpriteMapFactory.h"

using namespace std;
using namespace Escarabajo;

extern SpriteMap LevelFactory::spriteMap;
extern string LevelFactory::previousSpriteMapFilename;


Level* LevelFactory::load( string filename )
{
    Level* level = NULL;

    if ( Utilities::toLower( filename ).find( ".png" ) != string::npos )
    {
        // Loads a png level.
        LevelFileReaderPng reader( filename );
        LevelFileParser parser( reader, &spriteMap );

        level = parser.parse();
    }
    else
    {
        // Loads an old-school txt file level.
        LevelFileReader reader( filename );
        LevelFileParser parser( reader, &spriteMap );

        level = parser.parse();
    }

    return level;
}

void LevelFactory::loadSpriteMap( string filename )
{
    if ( filename == previousSpriteMapFilename )
    {
        // Oh, we already loaded this one.  KTHNXBYE.
        return;
    }

    spriteMap = SpriteMapFactory::load( filename );

    // Okay, well that worked out rather nicely.
    // Remember our filename so we only load when we
    // reeeeeaaly need to.
    previousSpriteMapFilename = filename;
}


Sprite* LevelFactory::getSprite( char name )
{
    return &spriteMap[name];
}

