#include "LevelFileParser.h"
#include "Utilities.h"
#include <sstream>

using namespace std;
using namespace Escarabajo;


// Sets the level file reader.
LevelFileParser::LevelFileParser( LevelFileReader& _reader, SpriteMap* _spriteMap )
	: reader(_reader)
{
	// Nothing else to do.
	spriteMap = _spriteMap;
}

// Sets the level file reader.
void LevelFileParser::setLevelFileReader( LevelFileReader& reader )
{
	this->reader = reader;
}

void LevelFileParser::setSpriteMap( SpriteMap* spriteMap )
{
	this->spriteMap = spriteMap;
}

// Parses the level.
Level* LevelFileParser::parse()
{
	Log::write( "LevelFileParser::parse() starting to parse level..." );

	// Get our data.
	vector<string> spriteSection = reader.getSection( LEVELFILE_SECTION_SPRITES );
	vector<string> structureSection = reader.getSection( LEVELFILE_SECTION_STRUCTURE );
	vector<string> valueSection = reader.getSection( LEVELFILE_SECTION_VALUES );

	// Throw errors if we need to
	if ( 0 == spriteSection.size() )
	{
		Log::write( "LevelFileParser::parse(): sprite section not found" );
		return NULL;
	}
	if ( 0 == structureSection.size() )
	{
		Log::write( "LevelFileParser::parse() structure section not found" );
		return NULL;
	}
	if ( spriteSection.size() != structureSection.size() )
	{
		Log::write( "LevelFileParser::parse(): mismatch in section sizes" );
		return NULL;
	}

	// Figure out how tall and wide our level is.
	int height = spriteSection.size();
	int width = spriteSection.front().length();

	stringstream s;
	s << "LevelFileParser::parse() dimensions: " << width << " x " << height;
	Log::write( s.str() );

	// Allocate level and copy data.
	Level* level = new Level( width, height );

	vector<string>::iterator it;

	// First do the sprites.
	{
		int y = 0;
		for ( it = spriteSection.begin(); it != spriteSection.end(); ++it )
		{
			string row = *it;
			for ( size_t x = 0; x < row.length(); ++x )
			{
				char c = row[x];
				map<char, Sprite>::const_iterator test = spriteMap->find( c );
				if ( test != spriteMap->end() )
				{
					Sprite* sprite = &(*spriteMap)[c];
					level->setSprite( sprite, x, y  );
				}
				else
				{
					//Log::write( "Sprite not found" );
				}
			}
			y++;
		}
	}

	// Now the structure.
	{
		int y = 0;
		for ( it = structureSection.begin(); it != structureSection.end(); ++it )
		{
			string row = *it;
			for ( size_t x = 0; x < row.length(); ++x )
			{
				char c = row[x];
				switch ( c )
				{
				case 'A' : // png
					level->setPiece( START, x, y );
					level->setStartingPosition( x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					break;
				case 'B' : // png
					level->setPiece( FINISH, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					break;
				case 'C' : // png
					level->setPiece( WALL, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					break;
				case 'D' : // png
				case 'E' : // png
				case 'F' : // png
					level->setPiece( CASTLE, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				case 'G' : // png
				case 'H' : // png
				case 'I' : // png
					level->setPiece( CASTLE_CRUMBLE, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				case 'J' :
					level->setPiece( CASTLE_CRUMBLE_WALL, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					break;
				case 'K' :
					level->setPiece( CASTLE1, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				case 'L' :
					level->setPiece( CASTLE2, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				case 'M' :
					level->setPiece( CASTLE3, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				case 'N' :
					level->setPiece( CASTLE4, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				case 'O' :
					level->setPiece( CASTLE5, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				case 'P' :
					level->setPiece( CASTLE6, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				case 'Q' :
					level->setPiece( CASTLE7, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				case 'R' :
					level->setPiece( CASTLE8, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				case 'S' :
					level->setPiece( CASTLE9, x, y );
					level->setAcceptDirections( x, y, false, false, false, false );
					level->setNumCastles( level->getNumCastles() + 1 );
					break;
				default :
					level->setPiece( EMPTY, x, y );
				}
			}
			y++;
		}
	}

	level->setRemainingCastles( level->getNumCastles() );

	// Middle section (special values)
	{
		string values = valueSection.at( 0 );
		int fuel = 3 * (int)values[ SECTION_VALUES_STARTING_FUEL ]; // 3 = fuel multiplier
		fuel += (int)values[ SECTION_VALUES_STARTING_FUEL_OFFSET ];
		level->setStartingFuel( fuel );
	}

	Log::write( "LevelFileParser::parse() ...parsing complete!" );

	return level;
}
