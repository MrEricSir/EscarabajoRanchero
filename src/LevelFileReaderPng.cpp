#include "LevelFileReaderPng.h"
#include "all.h"
#include "Utilities.h"
#include "Log.h"
#include <math.h>

using namespace std;
using namespace Escarabajo;


LevelFileReaderPng::LevelFileReaderPng()
{
    // Do NOTHING!!!
}

LevelFileReaderPng::LevelFileReaderPng( std::string filename )
{
    load( filename );
}

// Loads a file.
bool LevelFileReaderPng::load( std::string filename )
{
    // Unload previous file.
    sections.clear();

    string line = "";

    SDL_Surface* surface = IMG_Load( filename.c_str() );

    if ( !surface )
    {
        Log::write( "Error: couldn't load surface!" );
    }

    SDL_LockSurface(surface);

    // Calculate dividing line.
    int midpoint = floor( surface->h / 2.0 );

    Uint8* p = (Uint8*)surface->pixels;

    for ( int j = 0; j < surface->h; j++ )
    {
        // Clear our line buffer.
        line = "";

        // Advance pointer.
        p =  (Uint8*)surface->pixels + ( j * surface->pitch );

        // Iterate over the columns.
        for ( int i = 0; i < surface->w; i++ )
        {
            Uint8 myPix = *p;

            if ( midpoint != j )
            {
                // For every line but the middle, add a value
                // so that we map the 0..n to a character we can
                // represent in our .INI file.
                myPix += 64;
            }
            else
            {
                myPix += 1;
            }

            line += (char)( myPix );
            p += 1;
        }

        // Add to section.
        if ( j < midpoint )
        {
            // Structure.
            sections[ LEVELFILE_SECTION_STRUCTURE ].push_back( line );
        }
        else if ( j > midpoint )
        {
            // Sprites.
            sections[ LEVELFILE_SECTION_SPRITES ].push_back( line );
        }
        else
        {
            // Middle line (special values)
            sections[ LEVELFILE_SECTION_VALUES ].push_back( line );
        }
    }

    SDL_UnlockSurface( surface );
    SDL_FreeSurface( surface );

    return true;
}
