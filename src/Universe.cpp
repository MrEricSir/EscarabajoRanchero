#include "Universe.h"
#include "Log.h"

using namespace std;
using namespace Escarabajo;

vector<World*> Universe::worldList;
int Universe::currentWorldNumber = 0;

void Universe::init( std::string filename )
{
    // Do is our loading shiathe.
    UniverseFileReader reader( &worldList );
    reader.load( filename );
}

void Universe::deinit()
{
    vector<World*>::iterator it;
    for ( it = worldList.begin(); it != worldList.end(); ++it )
    {
        if ( *it != NULL )
        {
            delete (*it);
        }
        *it = NULL;
    }
}

int Universe::getNumWorlds()
{
    return worldList.size();
}

World* Universe::getWorld( int num )
{
    if ( num >= 0 && num < getNumWorlds() )
    {
        return worldList[ num ];
    }

    Log::write( "ERROR -- bad world number" );
    return NULL;
}

World* Universe::getCurrentWorld()
{
    return getWorld( currentWorldNumber );
}

int Universe::getCurrentWorldNum()
{
    return currentWorldNumber;
}

void Universe::setCurrentWorldNum( int num )
{
    currentWorldNumber = num;
}


Universe::UniverseFileReader::UniverseFileReader( std::vector<World*>* list )
{
    worlds = list;
}

void Universe::UniverseFileReader::section( std::string sectionName )
{
    currentWorld = new World; // But is it a brave new world?
    currentWorld->setName( sectionName );
    worlds->push_back( currentWorld );
}

void Universe::UniverseFileReader::pair( std::string name, std::string value )
{
    if ( "music" == name )
    {
        currentWorld->setMusicFilename( value );
    }
    else if ( "dir" == name )
    {
        currentDir = value;
    }
    else if ( "levels" == name )
    {
        Log::write( "Levels: " + value );
        vector<string> vec = Utilities::parseCSV( value );
        vector<string>::iterator it;
        for ( it = vec.begin(); it != vec.end(); ++it )
        {
            Level* l = LevelFactory::load( currentDir + *it );
            currentWorld->addLevel( l );
        }
    }
    else if ( "background" == name )
    {
        currentWorld->setBackgroundFilename( value );
    }

}


