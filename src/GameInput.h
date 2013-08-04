#ifndef GAMEINPUT_H
#define GAMEINPUT_H

#include "all.h"
#include "DispatchKeyHandler.h"
#include "DispatchEventHandler.h"
#include "MoveDirection.h"

#include <vector>

namespace Escarabajo
{

class GameInput : public Escarabajo::DispatchKeyHandler
{
    public:

    enum KeyState
    {
        KEYSTATE_RELEASED,     // Key is not active.
        KEYSTATE_PRESSED,      // Key is held down.
        KEYSTATE_PRESSED_READ, // Key is held down and game has acknolwedged it.
        KEYSTATE_RELEASING     // Key is going back to up state.
    };

    GameInput();
    ~GameInput();
    
    // Call these to begin/end input cycle.
    void inputBegin();
    void inputEnd();
    
    // Returns the desired player direction.
    // IMPORTANT: only call this once per tick at most.
    MoveDirection getDirection();
    
    bool shouldRestart();
    bool shouldQuit();

    virtual void pressLeft();
    virtual void releaseLeft();

    virtual void pressRight();
    virtual void releaseRight();


    virtual void pressUp();
    virtual void releaseUp();

    virtual void pressDown();
    virtual void releaseDown();

    virtual void pressEscape();
    virtual void pressPlus();
    virtual void pressMinus();
    virtual void pressR();
    virtual void pressP();

    private:

    void directionPressed( MoveDirection dir );
    void directionReleased( MoveDirection dir );
    
    // Returns true if the direction is in the stack.
    bool keyStackContains( MoveDirection dir );
    
    // Removes a direction from the key stack if it's present.
    void removeFromKeyStack( MoveDirection dir );

    // Stack of keys as they're held.
    std::vector<MoveDirection> keyStack;
    
    // Pressed state of direction keys.
    KeyState pressed[ DIRECTION_COUNT ];
    
    bool restart;
    bool quit;
};

}

#endif // GAMEINPUT_H
