#include "World.h"

using namespace Escarabajo;
using namespace std;

World::World()
{
    currentLevelNum = 0;
    background = NULL;
}

World::~World()
{
    if ( background )
    {
        delete background;
    }
}

void World::setMusicFilename( std::string filename )
{
    musicFilename = filename;
}

void World::setBackgroundFilename( std::string filename )
{
    backgroundFilename = Utilities::removeExtraSpaces( filename );
    if ( backgroundFilename != "" )
    {
        background = new Sprite();
        background->loadImage( backgroundFilename );
        vector<Level*>::iterator it;
        for ( it = levels.begin(); it != levels.end(); ++it )
        {
            (*it)->setBackgroundSprite( background );
        }
    }
}

std::string World::getMusicFilename()
{
    return musicFilename;
}

std::string World::getBackgroundFilename()
{
    return backgroundFilename;
}


void World::addLevel( Level* l )
{
    levels.push_back( l );
    l->setBackgroundSprite( background );
}

Level* World::getCurrentLevel()
{
    return getLevel( currentLevelNum );
}

Level* World::getLevel( int num )
{
    if ( num >= 0 && num < getNumLevels() )
    {
        return levels[num];
    }

    Log::write( "You've requested a bogus level, dude." );
    return NULL;
}

void World::setCurrentLevelNum( int num )
{
    currentLevelNum = num;
}

int World::getCurrentLevelNum()
{
    return currentLevelNum;
}

int World::getNumLevels()
{
    return levels.size();
}

void World::setName( std::string n )
{
    name = n;
}

std::string World::getName()
{
    return name;
}

