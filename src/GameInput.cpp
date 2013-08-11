#include "GameInput.h"
#include <algorithm>

namespace Escarabajo
{

GameInput::GameInput()
{
    restart = false;
    quit = false;
    
    for (int i = 0; i < DIRECTION_COUNT; i++)
        pressed[i] = KEYSTATE_RELEASED;
}

GameInput::~GameInput()
{
}

MoveDirection GameInput::getDirection()
{
    // Remove unset keys from stack.
    // TODO: This can be optimized.
    if ( pressed[MOVE_UP] == KEYSTATE_RELEASED )
        removeFromKeyStack( MOVE_UP );
    
    if ( pressed[MOVE_RIGHT] == KEYSTATE_RELEASED )
        removeFromKeyStack( MOVE_RIGHT );
    
    if ( pressed[MOVE_DOWN] == KEYSTATE_RELEASED )
        removeFromKeyStack( MOVE_DOWN );
    
    if ( pressed[MOVE_LEFT] == KEYSTATE_RELEASED )
        removeFromKeyStack( MOVE_LEFT );
    
    for ( int i = 0; i < DIRECTION_COUNT; i++ ) {
        // Acknolwedge keys that we've read.
        if ( pressed[i] == KEYSTATE_PRESSED )
            pressed[i] = KEYSTATE_PRESSED_READ;
        
        // Set keys that are releasing back to up state.
        // This allows us to handle keys that were briefly tapped
        if ( pressed[i] == KEYSTATE_RELEASING )
            pressed[i] = KEYSTATE_RELEASED;
    }
    
    // Grab whatever direction was most recently pressed.
    if (keyStack.size() == 0)
        return NONE;
    
    return keyStack.back();
}

bool GameInput::shouldRestart()
{ 
    bool ret = restart;
    restart = false;
    return ret;
}

bool GameInput::shouldQuit()
{ 
    bool ret = quit;
    quit = false;
    return ret;
}

void GameInput::pressEscape()
{
    quit = true;
}

void GameInput::pressLeft()
{
    directionPressed( MOVE_LEFT );
}
void GameInput::releaseLeft()
{
    directionReleased( MOVE_LEFT );
}

void GameInput::pressRight()
{
    directionPressed( MOVE_RIGHT );
}
void GameInput::releaseRight()
{
    directionReleased( MOVE_RIGHT );
}

void GameInput::pressUp()
{
    directionPressed( MOVE_UP );
}
void GameInput::releaseUp()
{
    directionReleased( MOVE_UP );
}

void GameInput::pressDown()
{
    directionPressed( MOVE_DOWN );
}
void GameInput::releaseDown()
{
   directionReleased( MOVE_DOWN );
}


void GameInput::pressPlus()
{
    //speedIncrease();
}

void GameInput::pressMinus()
{
    //speedDecrease();
}

void GameInput::pressR()
{
    restart = true;
}

void GameInput::pressP()
{
    //setPaused( !isPaused() );
}

void GameInput::directionPressed( MoveDirection dir )
{
    // Add to the stack if it's not there already.
    if (!keyStackContains( dir ))
        keyStack.push_back( dir );
    
    // Remember that this key is held down (unless we've already seen a tap release.)
    if ( pressed[ dir ] != KEYSTATE_RELEASING )
        pressed[ dir ] = KEYSTATE_PRESSED;
}

void GameInput::directionReleased( MoveDirection dir )
{
    // Key is no longer held down.
    if ( pressed[dir] == KEYSTATE_PRESSED_READ)
        pressed[ dir ] = KEYSTATE_RELEASED; // Already got this one.
    else
        pressed[ dir ] = KEYSTATE_RELEASING; // Must have been a quick tap.
}

bool GameInput::keyStackContains( MoveDirection dir )
{
    std::vector<MoveDirection>::iterator it;   
    it = std::find(keyStack.begin(), keyStack.end(), dir);
    return it != keyStack.end();
}

void GameInput::removeFromKeyStack( MoveDirection dir )
{
    std::vector<MoveDirection>::iterator it;   
    it = std::find(keyStack.begin(), keyStack.end(), dir);
    if (it != keyStack.end())
        keyStack.erase(it);
}


}
