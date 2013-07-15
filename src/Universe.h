#ifndef WORLDFACTORY_H_
#define WORLDFACTORY_H_

#include <vector>
#include <string>
#include "World.h"
#include "MapFileReader.h"
#include "LevelFactory.h"

namespace Escarabajo
{


// I think you'll find that the universe
// pretty much covers everything.
class Universe
{
public:

	// Initialize and load the universe.
	static void init( std::string filename );

	// Can I has my memory back?
	static void deinit();

	// Get the number of worlds in our universe.
	static int getNumWorlds();

	// Get a specific world, valid numbers are
	// [0, getNumWorlds()]
	static World* getWorld( int num );

	// Returns the current world.
	static World* getCurrentWorld();

	// Returns the current world number.
	static int getCurrentWorldNum();

	// Sets the current world.
	static void setCurrentWorldNum( int num );

private:

	// List of our worlds.
	static std::vector<World*> worldList;

	// Current world.
	static int currentWorldNumber;

	// Read our universe file reader.
	class UniverseFileReader : public MapFileReader
	{
	public:
		UniverseFileReader( std::vector<World*>* list );
		virtual void section( std::string sectionName );
		virtual void pair( std::string name, std::string value );
	private:
		std::vector<World*>* worlds;
		World* currentWorld;
		std::string currentDir;
	};
};

}

#endif /* WORLDFACTORY_H_ */
