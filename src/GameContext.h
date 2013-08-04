#ifndef GAMECONTEXT_H_
#define GAMECONTEXT_H_

#include "Level.h"
#include "all.h"
#include "GameObject.h"
#include "Log.h"
#include "Graphics.h"
#include "Config.h"
#include "CastleDetect.h"
#include "Array2D.h"
#include "MoveDirection.h"
#include "Audio.h"
#include "DispatchHandler.h"

namespace Escarabajo
{

class Input;
class GameObject;

class GameContext : public DispatchHandler
{
public:

    // Ctor.
    GameContext( unsigned intervalMs );

    // Dtor.
    virtual ~GameContext();

    // Okay, let's go!
    void start( void );

    // No wait, let's stop.
    virtual bool quit();

    // Restarts the level.
    void restartLevel( void );

    // Get graphics object.
    Graphics getGraphics();

    // Add an object.
    void addObject( GameObject* object );

    // Render frame.
    void render( void );

    // Animate.
    void animate();

    // Eventually we'll delete this shit.
    void setObjectForInput( GameObject* object );

    // Change to a specific level.
    void setLevel( int worldNum, int levelNum );

    int getCurrentWorldNum();

    int getCurrentLevelNum();

    // Go to the next level.
    void incrementLevel();

    // Get the level.  Be careful
    // not to delete the pointer or anything...
    Level* getLevel();

    // Run detection.
    void runDetection( int x, int y );

    // Sets paused/unpaused (true, false)
    void setPaused( bool p );

    // True if we're paused or in a transitional state.
    inline bool isPaused()
    {
        return paused || transition;
    }

    // DEATH!!!!
    void die();

    // Yuo = teh winrar
    void winLevel();

    // Decreases fuel.
    void decrementFuel();

    void speedIncrease();

    void speedDecrease();

    void loadFont( std::string filename );

    GameFont* getFont();

    // Instance of game loop.
    virtual void tick();

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

    // Sets the level.  Cleans up the old level data
    // and resets everything as necessary.
    void setLevelInternal( Level* level );

    // Amount of time left.
    unsigned int timeLeft( void );

    // Run the recursive detection routine.
    void runDetectionInternal( int x, int y );

    // Called by runDetection when a castle
    // area is found.
    // Add (or) our mask and remove the castle.
    bool addMask( Array2D<bool>* m );

    // Get a castle order.  Returns false if
    // the castle order caused us to DIE!
    bool doCastleOrder( int value );

    // Draws the score.
    void drawScore();

    // Swaps out animated mask bits with static ones.
    void advanceMask();

    void inputReset();

    bool canApplyMotion();

    void applyMotion();

    bool checkAllow( MoveDirection inputDir );

    void doLegacyInput();

    // Graphics object.
    Graphics graphics;

    // Our main font.
    GameFont font;

    // Level data: this is a copy of
    // the level for us to work with.
    Level* level;

    // Level castle mask.
    Array2D<int>* mask;

    // Objects.
    std::vector<GameObject*> objectList;

    // True while the game is running.
    bool running;

    // Castle detection logic.
    CastleDetect* detect;

    // Mask sprites.
    Sprite* spriteMask;
    Sprite* spriteMaskAnim;

    // Paused variable (true if paused.)
    bool paused;

    // Transition -- a pause caused by a
    // level transition or some such nonsense.
    // True if we should be paused.
    bool transition;

    int tickInterval;

    int lastCastleOrder;

    // Our main game object that we're moving around.
    GameObject* object;

    // Indicates the user has pressed one of these.
    bool isPressUp;
    bool isPressDown;
    bool isPressLeft;
    bool isPressRight;
    
    // Indicates the user has released one of these.
    // Key releases are handled during the update loop to allow for quick key tapping.
    bool isReleaseUp;
    bool isReleaseDown;
    bool isReleaseLeft;
    bool isReleaseRight;

    // Direciton we're facing in.
    MoveDirection facingDirection;

    // Should we accept directional input?
    bool directionLock;

    int moveAmount;

    bool shouldMove;
    
    bool prevCanApplyMotion;

};

}

#endif /* GAMECONTEXT_H_ */
