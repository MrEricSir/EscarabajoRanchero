#ifndef LEVELFACTORY_H_
#define LEVELFACTORY_H_

#include "all.h"
#include "Level.h"
#include "Utilities.h"
#include "Sprite.h"
#include "LevelFileReader.h"
#include "LevelFileReaderPng.h"
#include "LevelFileParser.h"
#include "SpriteMap.h"

namespace Escarabajo
{

class Level;

class LevelFactory
{

private:

	// Sprite map.
	static SpriteMap spriteMap;

	// THe previous file we loaded.
	static std::string previousSpriteMapFilename;


public:

	// Loads a level from a level file.
	// Prerequisite: loadSpriteMap() must be called first.
	static Level* load( std::string filename );

	// Loads a sprite map file and the associated sprites.
	static void loadSpriteMap( std::string filename );

	static Sprite* getSprite( char name );

};

}

#endif /* LEVELFACTORY_H_ */
