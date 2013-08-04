#include "GameContext.h"
#include "LevelFactory.h"
#include "AudioSamples.h"
#include "Universe.h"
#include "GameSettings.h"

using namespace std;
using namespace Escarabajo;

GameContext::GameContext( unsigned intervalMs ) : DispatchHandler( intervalMs )
{
    facingDirection = MOVE_UP;
    level = NULL;
    detect = NULL;
    mask = NULL;
    spriteMask = NULL;
    spriteMaskAnim = NULL;
    paused = false;
    transition = false;
    tickInterval = 120 / 15;// 6 frames per move?
    lastCastleOrder = 0;
    shouldMove = false;
}


GameContext::~GameContext()
{
    // Remove our detection object.
    if ( NULL != detect )
    {
        delete detect;
        detect = NULL;
    }

    // Remove level copy.
    if ( NULL != level )
    {
        delete level;
        level = NULL;
    }

    // Remove mask
    if ( NULL != mask )
    {
        delete mask;
        mask = NULL;
    }
}


Graphics GameContext::getGraphics()
{
    return this->graphics;
}

void GameContext::addObject( GameObject* object )
{
    objectList.push_back( object );
}

void GameContext::inputReset( void )
{
    facingDirection = MOVE_UP;
    isPressUp = false;
    isPressDown = false;
    isPressRight = false;
    isPressLeft = false;
    directionLock = false;
    
    isReleaseUp = false;
    isReleaseDown = false;
    isReleaseRight = false;
    isReleaseLeft = false;
    
    shouldMove = false;
    transition = false;
}

// Reset a bunch of shit!!!
void GameContext::setLevelInternal( Level* l )
{
    // Remove our level copy.
    if ( this->level != NULL )
    {
        Log::write( "setLevelInternal: Removing old level" );
        delete this->level;
    }

    Log::write( "setLevelInternal: Cloning level" );
    this->level = l->clone();

    if ( mask != NULL )
    {
        // Remove previous mask.
        Log::write( "setLevelInternal: Removing old mask" );
        delete mask;
    }
    mask = new Array2D<int>( level->getWidth9(), level->getHeight9() );
    mask->setAll( 0 );

    lastCastleOrder = 0;

    if ( detect != NULL )
    {
        // Remove our detection object, if need be.
        Log::write( "setLevelInternal: Removing old detect object" );
        delete detect;
    }

    Log::write( "Creating castle detect" );
    this->detect = new CastleDetect( this->level );

    inputReset();
    if ( object )
    {
        object->setLocation( level->getStartingX(), level->getStartingY() );
    }

    Log::write( "Level is set" );
    Audio::playSound( AUDIO_LEVEL_START );
}

Level* GameContext::getLevel()
{
    return this->level;
}

void GameContext::setLevel( int worldNum, int levelNum )
{
    Log::write( "Changing level to " + Utilities::longToString( worldNum )
                 + " " + Utilities::longToString( levelNum ) );
    World* w = Universe::getWorld( worldNum );
    if ( NULL == w )
    {
        Log::write( "ERROR --- World is null!" );
        return;
    }
    Level * l = w->getLevel( levelNum );
    if ( NULL == l )
    {
        Log::write( "ERROR --- Level is null!" );
        return;
    }
    setLevelInternal( l );
}

void GameContext::incrementLevel()
{
    int curLevelNum =  Universe::getCurrentWorld()->getCurrentLevelNum();
    int totalLevels = Universe::getCurrentWorld()->getNumLevels();

    // If there's another level in this world, just grab that.
    curLevelNum++;
    if ( curLevelNum < totalLevels )
    {
        Universe::getCurrentWorld()->setCurrentLevelNum( curLevelNum );
        Level* l = Universe::getCurrentWorld()->getCurrentLevel();
        setLevelInternal( l );
        return;
    }

    // No more levels in this world, try to get
    // the next world then.
    if ( Universe::getNumWorlds() <= Universe::getCurrentWorldNum() + 1 )
    {
        Log::write( "+++++++++++++ WIN +++++++++++++" );
        return;
    }
    else
    {
        Universe::setCurrentWorldNum( Universe::getCurrentWorldNum() + 1 );
        Level* l = Universe::getCurrentWorld()->getCurrentLevel();
        setLevelInternal( l );
    }
}

int GameContext::getCurrentWorldNum()
{
    return Universe::getCurrentWorldNum();
}

int GameContext::getCurrentLevelNum()
{
    return Universe::getCurrentWorld()->getCurrentLevelNum();
}

void GameContext::advanceMask()
{
    for ( int y = 0; y < mask->getHeight(); ++y )
    {
        for ( int x = 0; x < mask->getWidth(); ++x )
        {
            if ( mask->get( x, y ) == 1 )
            {
                mask->set( 2, x, y );
            }
        }
    }
}

void GameContext::render( void )
{
    graphics.beginRendering();

    // Let's start with our level.
    level->draw();

    // Draw our mask.
    for ( int y = 0; y < mask->getHeight(); ++y )
    {
        for ( int x = 0; x < mask->getWidth(); ++x )
        {
            int m = mask->get( x, y ) ;
            if ( 0 == m )
            {
                continue;
            }
            else if ( 1 == m )
            {
                // Animated sprite mask.
                spriteMaskAnim->draw( x * Config::getWidthOfLevelPiece9(), y * Config::getHeightOfLevelPiece9(), -1 );
            }
            else
            {
                // Static sprite mask.
                spriteMask->draw( x * Config::getWidthOfLevelPiece9(), y * Config::getHeightOfLevelPiece9(), -1 );
            }
        }
    }

    // Now go through our object list...
    vector<GameObject*>::iterator objectIter;
    for( objectIter=objectList.begin(); objectIter!=objectList.end(); objectIter++ )
    {
        (*objectIter)->draw();
    }

    drawScore();

    graphics.endRendering();
}




void GameContext::animate()
{
    // Animate game object if moving.
    if ( canApplyMotion() )
    {
        // Now go through our object list...
        vector<GameObject*>::iterator objectIter;
        for( objectIter=objectList.begin(); objectIter!=objectList.end(); objectIter++ )
        {
            (*objectIter)->animateObject();
            (*objectIter)->animateMotion();
        }
    }

    // Tell level to animate.
    level->animate();

    // Animate the sprite mask.
    spriteMaskAnim->animAdvance();
}


void GameContext::drawScore()
{
    // TODO: optimize this.  No need to re-create strings every time!
    font.draw( 12, 228, 11, "silk:" );
    font.draw( 132, 228, 11, "food: " + Utilities::longToString( level->getRemainingCastles() )
                            + "/" + Utilities::longToString( level->getNumCastles() )  );

    font.draw( 212, 228, 11, "elapsed: " + Utilities::longToString( level->getElapsedTime() ) );


    // Remaining fuel.
    Sprite* silk = LevelFactory::getSprite( TRAIL_HORIZONTAL );
    int x = 50;
    int y = 225;
    Utilities::clippingRectSet( x, y,
                level->getRemainingFuel(), Config::getHeightOfLevelPiece() );
    for ( int i = 0; i < 4; ++i )
    {
        silk->draw( x, y, 11 );
        x += Config::getWidthOfLevelPiece();
    }
    Utilities::clippingRectUnset();
}

void GameContext::start( void )
{
    // Uh yeah.
    running = true;

    // Initialize!
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO );

    moveAmount = 1;

    // Init openGL.
    Graphics graphics;
    graphics.setWindowTitle( "Escarabajo Ranchero" );
    graphics.setSize( Config::getScreenWidth(), Config::getScreenHeight() );
    graphics.init();

    // Audio/music.
    Audio::init();
    Audio::loadSoundMap( "resources/audio/map.txt" );
    Audio::loadMusicMap( "resources/music/map.txt" );

    // Load graphics.
    LevelFactory::loadSpriteMap( "resources/img/map.txt" );

    // Get our mask sprite.
    // TODO: move to level?
    spriteMask = LevelFactory::getSprite( 'm' );
    spriteMaskAnim = LevelFactory::getSprite( 'e' );

    // TEMP:
    // start music.
    Audio::playMusic( "testSong" );
}

bool GameContext::quit( void )
{
    Log::write( "Quit was called" );

    // Stop running.
    running = false;

    Audio::deinit();

    return false; // exit the game.
}

void GameContext::restartLevel( void )
{
    setLevelInternal( Universe::getCurrentWorld()->getCurrentLevel() );
}


void GameContext::setObjectForInput( GameObject* object )
{
    this->object = object;
    object->setX( level->getStartingX() );
    object->setY( level->getStartingY() );
}


void GameContext::speedDecrease()
{
    if ( tickInterval < 2000 )
    {
        tickInterval += 40;
    }
}


void GameContext::speedIncrease()
{
    if ( tickInterval > 20 )
    {
        tickInterval -= 40;
    }
}


unsigned int GameContext::timeLeft( void )
{
    static unsigned int next_time = 0;
    unsigned int now;

    now = SDL_GetTicks();
    if ( next_time <= now )
    {
        next_time = now + tickInterval;
    }
    return next_time - now;
}

void GameContext::runDetection( int x, int y )
{
    Log::write("Running detection: " +
            Utilities::longToString( x ) + ", " + Utilities::longToString( y ) );

    // Replace animated mask tiles with static ones.
    advanceMask();

    detect->resetDetection();

    // Run detection.
    // TODO: optimize!  This may not be need to run
    //       in all four directions.
    int mx = x * 3 + 1;
    int my = y * 3 + 1;
    runDetectionInternal( mx - 1, my + 1 );
    runDetectionInternal( mx - 1, my - 1 );
    runDetectionInternal( mx + 1, my + 1 );
    runDetectionInternal( mx + 1, my - 1 );

    detect->mergeUsedSnakes();
}

void GameContext::runDetectionInternal( int x, int y )
{
    if ( detect->runDetection( x, y ) )
    {
        Log::write( "Castle found!" );

        // We got a new mask, add it.
        Array2D<bool>* m = detect->getMask();
        if ( addMask( m ) )
        {
            // Play a little sound.
            Audio::playSound( AUDIO_LASSO_COMPLETE );

            // Reset animated sprite mask.
            spriteMaskAnim->animReset();

            // Increment fuel by 9.
            int fuel = level->getRemainingFuel();
            fuel += 9;
            level->setRemainingFuel( fuel );

            return;
        }
    }

    // Well you crossed a path, otherwise we wouldn't
    // be in this function.  So play a different sound.
    Audio::playSound( AUDIO_TRAIL_CROSS );
}

bool GameContext::doCastleOrder( int value )
{
    if ( value == lastCastleOrder + 1 )
    {
        // Right castle, so increment.
        ++lastCastleOrder;
        return true;
    }
    else
    {
        // FAIL: you went around the wrong castle.
        Log::write( "OOPS!  Wrong castle!" );
        return false;
    }
}

bool GameContext::addMask( Array2D<bool>* m )
{
    bool ret = false;
    // Find castle.
    for ( int y = 0; y < mask->getHeight(); ++y )
    {
        for ( int x = 0; x < mask->getWidth(); ++x )
        {
            int dx = floor( (double)x / 3.0 );
            int dy = floor( (double)y / 3.0 );
            if ( m->get(x, y) )
            {
                // Remove castle.
                if ( level->getPiece( dx, dy ) == CASTLE )
                {
                    level->setRemainingCastles( level->getRemainingCastles() - 1 );
                    level->setPiece( WALL, dx, dy );
                    ret = true;
                }
                if ( level->getPiece( dx, dy ) >= CASTLE1 &&
                     level->getPiece( dx, dy ) <= CASTLE9 )
                {
                    // Figure out the "value" of the castle.
                    int castleVal = level->getPiece( dx, dy ) - CASTLE1 + 1;

                    // Eliminate the castle first, so that even with
                    // incorrect ordering it still gets "removed."
                    level->setPiece( WALL, dx, dy );

                    // Make sure order was correct.
                    if ( !doCastleOrder( castleVal ) ) return false;

                    // It was, yay!  Reduce remaining castles and return true.
                    ret = true;
                    level->setRemainingCastles( level->getRemainingCastles() - 1 );
                }
                else if ( level->getPiece( dx, dy ) == CASTLE_CRUMBLE )
                {
                    ret = true;
                    Log::write( "Castle crumbled to nothingness" );
                    level->setRemainingCastles( level->getRemainingCastles() - 1 );
                    level->setPiece( EMPTY, dx, dy );
                    level->setSprite( NULL, dx, dy );
                    level->setAcceptDirections( dx, dy, true, true, true, true );
                }
                else if ( level->getPiece( dx, dy ) == CASTLE_CRUMBLE_WALL )
                {
                    level->setPiece( EMPTY, dx, dy );
                    level->setSprite( NULL, dx, dy );
                    level->setAcceptDirections( dx, dy, true, true, true, true );
                }
            }
        }
    }

    // Add to our mask.
    for ( int y = 0; y < mask->getHeight(); ++y )
    {
        for ( int x = 0; x < mask->getWidth(); ++x )
        {
            if ( m->get(x, y) )
            {
                // Add to our mask.
                mask->set( 1, x, y );
            }
        }
    }

    return ret;
}


void GameContext::setPaused( bool p )
{
    paused = p;
    if ( paused )
    {
        Audio::playSound( AUDIO_PAUSE );
    }
}

void GameContext::die()
{
    Log::write( "GameContext: You have DIED!!!!" );
#if ALLOW_LEVEL_END
    transition = true;
    level->setComplete( true );
    Audio::playSound( AUDIO_PLAYER_DEATH );
    SDL_Delay( 500 ); // some amount of time?
    restartLevel();
#endif

}

void GameContext::winLevel()
{
    Log::write( "GameContext: win!!!!" );
#if ALLOW_LEVEL_END
    transition = true;
    level->setComplete( true );
    Audio::playSound( AUDIO_PLAYER_WIN );
    SDL_Delay( 500 ); // some amount of time?
    incrementLevel();
#endif
}



void GameContext::tick()
{
    // Do animation and graphics.
    static int STEPMAX = Config::getAnimationStep() - 1;
    static int step = 0;
    bool doMotion = false;
    
    // Handle input.
    doLegacyInput();
    
    // Render,
    render();

    // Animate.
    animate();
    
    if (step == STEPMAX)
        doMotion = true;

    // Handle motion.
    if ( canApplyMotion() )
    {
        if ( doMotion )
        {
            //directionLock = false;
            applyMotion();

            // Draw snake.
            getLevel()->drawSnake( object->getX(), object->getY(), facingDirection );
            step = 0;

            directionLock = false;
        }
        else
        {
            step++;
        }
    }
    else
    {
        directionLock = false;
    }
    
    // Deal with key releases here to allow quick taps to affect the above loop.
    if (isReleaseUp)
        isPressUp = false;
    if (isReleaseDown)
        isPressDown = false;
    if (isReleaseRight)
        isPressRight = false;
    if (isReleaseLeft)
        isPressLeft = false;
    
    // Release all when we're at a sprite boundary.
    if ( doMotion )
    {
        isReleaseUp = false;
        isReleaseDown = false;
        isReleaseRight = false;
        isReleaseLeft = false;
    }
}


void GameContext::decrementFuel()
{
    int f = level->getRemainingFuel();
    if ( 0 == f )
    {
        die();
    }
    else
    {
        level->setRemainingFuel( f - 1 );
    }
}


void GameContext::loadFont( std::string filename )
{
    font.load( filename );
}

GameFont* GameContext::getFont()
{
    return &font;
}

void GameContext::pressEscape()
{
    SDL_Event event;
    event.type = SDL_QUIT;

    SDL_PushEvent(&event);
}

void GameContext::pressLeft()
{
    if ( checkAllow( MOVE_LEFT ) )
    {
        isPressLeft = true;
    }
}
void GameContext::releaseLeft()
{
    //isPressLeft = false;
    isReleaseLeft = true;
}

void GameContext::pressRight()
{
    if ( checkAllow( MOVE_RIGHT ) )
    {
        isPressRight = true;
    }
}
void GameContext::releaseRight()
{
    //isPressRight = false;
    isReleaseRight = true;
}


void GameContext::pressUp()
{
    if ( checkAllow( MOVE_UP ) )
    {
        isPressUp = true;
    }
}
void GameContext::releaseUp()
{
    //isPressUp = false;
    isReleaseUp = true;
}

void GameContext::pressDown()
{
    if ( checkAllow( MOVE_DOWN ) )
    {
        isPressDown = true;
    }
}
void GameContext::releaseDown()
{
    //isPressDown = false;
    isReleaseDown = true;
}


void GameContext::pressPlus()
{
    speedIncrease();
}

void GameContext::pressMinus()
{
    speedDecrease();
}

void GameContext::pressR()
{
    if ( !isPaused() )
    {
        restartLevel();
    }
}

void GameContext::pressP()
{
    setPaused( !isPaused() );
}


bool GameContext::canApplyMotion()
{
    MoveDirection direction = facingDirection;

    if ( !shouldMove )
    {
        return false;
    }

    if ( MOVE_UP == direction )
    {
        int newY = object->getY() - moveAmount;
        return getLevel()->canMoveTo( object->getX(), newY, direction );
    }

    if ( MOVE_DOWN == direction )
    {
        int newY = object->getY() + moveAmount;
        return getLevel()->canMoveTo( object->getX(), newY, direction );
    }

    if ( MOVE_RIGHT == direction )
    {
        int newX = object->getX() + moveAmount;
        return getLevel()->canMoveTo( newX, object->getY(), direction );
    }

    if ( MOVE_LEFT == direction )
    {
        int newX = object->getX() - moveAmount;
        return getLevel()->canMoveTo( newX, object->getY(), direction );
    }

    return false;
}


void GameContext::applyMotion()
{
    // Okay now MOVE!
    if ( shouldMove )
    {

        MoveDirection direction = facingDirection;
        if ( MOVE_UP == direction )
        {
            int newY = object->getY() - moveAmount;
            if ( newY < 0 )
            {
                object->setY( getLevel()->getHeight() - 1 );
            }
            else if ( getLevel()->canMoveTo( object->getX(), newY, direction ) )
            {
                object->setY( newY );
            }
            else
            {
                shouldMove = false;
                return;
            }
        }

        if ( MOVE_DOWN == direction )
        {
            int newY = object->getY() + moveAmount;
            if ( newY >= getLevel()->getHeight() )
            {
                // Wrap.
                object->setY( 0 );
            }
            else if ( getLevel()->canMoveTo( object->getX(), newY, direction ) )
            {
                // Move;
                object->setY( newY );
            }
            else
            {
                shouldMove = false;
                return;
            }
        }

        if ( MOVE_RIGHT == direction )
        {
            int newX = object->getX() + moveAmount;
            if ( newX >= getLevel()->getWidth() )
            {
                object->setX( 0 );
            }
            else if ( getLevel()->canMoveTo( newX, object->getY(), direction ) )
            {
                object->setX( newX );
            }
            else
            {
                shouldMove = false;
                return;
            }
        }

        if ( MOVE_LEFT == direction )
        {
            int newX = object->getX() - moveAmount;
            if ( newX < 0 )
            {
                object->setX( getLevel()->getWidth() - 1 );
            }
            else if ( getLevel()->canMoveTo( newX, object->getY(), direction ) )
            {
                object->setX( newX );
            }
            else
            {
                shouldMove = false;
                return;
            }
        }
    }

    // Check if you won!
    if
    (
        0 == getLevel()->getRemainingCastles() &&
        getLevel()->getPiece( object->getX(), object->getY() ) == FINISH
    )
    {
        // YUO = TEH WINRAR!
        winLevel();
    }


    if ( transition || !shouldMove )
    {
        shouldMove = false;
        return;
    }

    // Check if we crossed an existing path.
    if ( SNAKE == getLevel()->getPiece( object->getX(), object->getY() ) )
    {
        // Compute snake.
        getLevel()->drawSnake( object->getX(), object->getY(), facingDirection );
        runDetection( object->getX(), object->getY() );
    }

    shouldMove = false;

    // Decrement fuel.
    decrementFuel();
}



bool GameContext::checkAllow( MoveDirection inputDir )
{
    int x = object->getX();
    int y = object->getY();
    Level* level = getLevel();

    switch ( inputDir )
    {
        case MOVE_RIGHT:
            if ( MOVE_LEFT == facingDirection ) return false;
            if ( SNAKE == level->getPieceWithWrap( x + 1, y ) &&
                !level->canMoveTo( x + 1, y, MOVE_RIGHT ) )
            {
                return false;
            }
            break;
        case MOVE_LEFT:
            if ( MOVE_RIGHT == facingDirection ) return false;
            if ( SNAKE == level->getPieceWithWrap( x - 1, y ) &&
                !level->canMoveTo( x - 1, y, MOVE_LEFT ) )
            {
                return false;
            }
            break;
        case MOVE_UP:
            if ( MOVE_DOWN == facingDirection ) return false;
            if ( SNAKE == level->getPieceWithWrap( x, y - 1 ) &&
                !level->canMoveTo( x, y - 1, MOVE_UP ) )
            {
                return false;
            }
            break;
        case MOVE_DOWN:
            if ( MOVE_UP == facingDirection ) return false;
            if ( SNAKE == level->getPieceWithWrap( x, y + 1 ) &&
                !level->canMoveTo( x, y + 1, MOVE_DOWN ) )
            {
                return false;
            }
            break;
    }

    return true;
}


void GameContext::doLegacyInput()
{
    // Now the fun part!
    // Don't do anything if we're paused or done.
    if ( !isPaused() && !getLevel()->isComplete() )
    {
        if ( !directionLock )
        {
            if ( isPressRight )
            {
                facingDirection = MOVE_RIGHT;
                shouldMove = true;
                directionLock = true;
            }
            else if ( isPressDown )
            {
                facingDirection = MOVE_DOWN;
                shouldMove = true;
                directionLock = true;
            }
            else if ( isPressLeft )
            {
                facingDirection = MOVE_LEFT;
                shouldMove = true;
                directionLock = true;
            }
            else if ( isPressUp )
            {
                facingDirection = MOVE_UP;
                shouldMove = true;
                directionLock = true;
            }
        }

        // Change direction.
        object->setDirection( facingDirection );

        // Compute snake.
        getLevel()->drawSnake( object->getX(), object->getY(), facingDirection );
    }
}
