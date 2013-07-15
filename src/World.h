#ifndef WORLD_H_
#define WORLD_H_

#include <string>
#include <vector>
#include "Level.h"
#include "Audio.h"

namespace Escarabajo
{

class Level;
class Sprite;

/**
 * Represents a WORLD, dawg.  WEST SIDE!
 */
class World
{

public:

	World();

	~World();

	void setMusicFilename( std::string filename );

	void setBackgroundFilename( std::string filename );

	void addLevel( Level* l );

	Level* getCurrentLevel();

	std::string getMusicFilename();

	std::string getBackgroundFilename();

	Sprite* getBackgroundSprite();

	Level* getLevel( int num );

	// Sets the current level.
	void setCurrentLevelNum( int num );

	int getCurrentLevelNum();

	// Gets the number of levels.
	int getNumLevels();

	void setName( std::string n );

	std::string getName();

private:

	std::string name;

	// Filename of our musics.
	std::string musicFilename;

	// Filename of our background image.
	std::string backgroundFilename;

	// Sweet sweet levelaid.
	std::vector<Level*> levels;

	// Current level.
	int currentLevelNum;

	// Sprite that represents the background.
	Sprite* background;

};

}

#endif
