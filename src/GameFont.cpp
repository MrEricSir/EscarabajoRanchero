#include "GameFont.h"
#include <iostream>

using namespace std;
using namespace Escarabajo;


/** Loads a font from a map file.
 */
void GameFont::load( std::string filename )
{
	spriteMap = SpriteMapFactory::load( filename );
}

void GameFont::draw( int x, int y, char c )
{
	draw( x, y, defaultZ, c );
}

void GameFont::draw( int x, int y, std::string c )
{
	draw( x, y, defaultZ, c );
}

void GameFont::draw( int x, int y, int z, char c )
{
	SpriteMap::iterator iterator = spriteMap.find( c );
	if ( iterator != spriteMap.end() )
	{
		iterator->second.draw( x, y, z );
	}
}

/** Draws a string.
 */
void GameFont::draw( int x, int y, int z, std::string c )
{
	size_t i = 0;
	while ( i < c.length() )
	{
		draw( x, y, z, c[i] );
		x += Config::getWidthOfFontPiece();
		i++;
	}
}
