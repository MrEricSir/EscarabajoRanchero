#include "all.h"
#include "GameContext.h"
#include "Level.h"
#include "Sprite.h"
#include "GameObject.h"
#include "LevelFactory.h"
#include "MoveDirection.h"
#include "Universe.h"
#include "GameSettings.h"
#include "Dispatcher.h"

using namespace std;
using namespace Escarabajo;

#define GAME_RATE 100

GameContext game( GAME_RATE );
Dispatcher dispatcher;


// Starts a level!
void startGame( int world, int level )
{
    // Take care of our level.
    // Note that this must be done after Universe::init and
    // before adding our game object.
    Log::write( "Setting level" );
    game.setLevel( world, level );

    // Create object for our sprite.
    Log::write( "Creating sprite" );
    GameObject obj;
    obj.setSprite( MOVE_UP, LevelFactory::getSprite( 'u' ) );
    obj.setSprite( MOVE_DOWN, LevelFactory::getSprite( 'i' ) );
    obj.setSprite( MOVE_LEFT, LevelFactory::getSprite( 'o' ) );
    obj.setSprite( MOVE_RIGHT, LevelFactory::getSprite( 'p' ) );

    // Add object.
    game.setObjectForInput( &obj );
    game.addObject( &obj );

    // Main loop.
    dispatcher.setDispatchHandler( &game );
    dispatcher.mainLoop();
}


/**
 * Seems like a great place to start, eh?
 *    - Canadian Commenter
 */
int main(int argc, char *argv[])
{
    long startWorld = 0; // world to start at
    long startLevel = 0; // level to start at

    if ( argc >= 2 )
    {
        string sOption = argv[1];
        if ( sOption == "-?" )
        {
            cout << "Command line help" << endl;
            cout << "-?                               Prints this help screen" << endl;
            cout << "-j [worldNumber] [levelNumber]   Jumps directly to a level" << endl;
            cout << endl;
            return 0;
        }
        if ( sOption == "-j" )
        {
            // Jump to level.
            string sWorld = argv[2];
            string sLevel = argv[3];
            startWorld = Utilities::stringToLong( sWorld );
            startLevel = Utilities::stringToLong( sLevel );
        }
    }

    GameSettings::load();

    // Init config.
    Config::init();

    // Start the game!
    game.start();

    // Load all the game data.
    Universe::init( "resources/level/worlds.txt" );
    game.loadFont( "resources/font/map.txt" );


    // todo: put this somewheres
    startGame( startWorld, startLevel );

    Universe::deinit();

    return 0;
}

