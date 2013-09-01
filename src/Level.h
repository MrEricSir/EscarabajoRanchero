#ifndef LEVEL_H_
#define LEVEL_H_

#include "all.h"
#include "LevelPiece.h"
#include "Sprite.h"
#include "Array2D.h"
#include "GameFont.h"
#include "Utilities.h"
#include "MoveDirection.h"
#include "TrailSprites.h"
#include "LevelFactory.h"

#include <math.h>

#define LEVEL_MAX_FUEL 60 // 20 is 1:1

namespace Escarabajo
{

class LevelUnit
{
public:
    LevelUnit()
    {
        piece = EMPTY;
        acceptTop = true;
        acceptBottom = true;
        acceptLeft = true;
        acceptRight = true;
        trailChar = TRAIL_NONE;
    }

    void makeSolid()
    {
        acceptTop = false;
        acceptBottom = false;
        acceptLeft = false;
        acceptRight = false;
    }

    LevelPiece piece;

    char trailChar;

    bool acceptTop;
    bool acceptBottom;
    bool acceptRight;
    bool acceptLeft;
};

/**
 * Defines a single level in the game.
 */
class Level
{

public:

    /**
     * Clears memory, does not load level.
     */
    Level();

    /**
     * Creates the level, width and height
     * in terms of the number of pieces.
     */
    Level( int width, int height );


    /**
     * Removes any data.
     */
    ~Level();

    /**
     * Sets a reference to the background sprite.
     */
    void setBackgroundSprite( Sprite* s );

    /**
     * Sets a piece for a given location.
     * Note: level must be initialized for this to work!
     */
    void setPiece( const LevelPiece& p, const int& x, const int& y );

    /**
     * Gets a piece for a given location.
     */
    LevelPiece getPiece( const int& x, const int& y );

    /**
     * True if the unit is a snake.
     */
    inline LevelPiece getPiece9( const int& sx, const int& sy )
    {
        LevelPiece test = snakeArray->get( sx, sy );
        if ( test != EMPTY )
        {
            return test;
        }

        int dx = floor( (double)sx / 3.0 );
        int dy = floor( (double)sy / 3.0 );
        return levelArray->get( dx, dy ).piece;
    }

    /**
     * Gets a piece with wrap-around.
     */
    LevelPiece getPieceWithWrap( int x, int y );

    /**
     * Set the starting position.
     */
    void setStartingPosition( const int& x, const int& y );

    /**
     * Gets the starting X coordinate.
     */
    int getStartingX();

    /**
     * Gets the starting Y coordinate.
     */
    int getStartingY();

    /**
     * Sets a sprite for a given location.
     * Note: level must be initialized for this to work!
     */
    void setSprite( Sprite* s, const int& x, const int& y );
    
    void drawBackground();

    /**
     * Draws the level.
     */
    void draw();


    // Gets level width.
    int getWidth();

    // Gets level height.
    int getHeight();

    inline int getWidth9() { return width * 3; }
    inline int getHeight9() { return height * 3; }

    // Gets the number of remaining castles.
    inline int getRemainingCastles()
    {
        return remainingCastles;
    }

    // Sets the number of remaining castles.
    inline void setRemainingCastles( int c )
    {
        remainingCastles = c;
        Log::write( "Remaining castles: " + Utilities::longToString( c ) );
        if ( !remainingCastles )
        {
            openExit();
        }
    }

    // Gets the total number of castles.
    inline int getNumCastles()
    {
        return numCastles;
    }

    // Sets the total number of castles.
    inline void setNumCastles( int c )
    {
        numCastles = c;
    }

    // Check if the tile can be moved to.
    bool canMoveTo( int x, int y, MoveDirection dir );

    // Sets the accept dirs.
    void setAcceptDirections( int x, int y, bool acceptTop, bool acceptBottom, bool acceptRight, bool acceptLeft );

    // Makes a deep copy of the level.
    Level* clone();

    // Check whether the level is complete.
    inline bool isComplete()
    {
        return complete;
    }

    // Change complete/incomplete status.
    void setComplete( bool c );

    // Draws a trail piece for a given square and direction.
    void drawSnake( int x, int y, MoveDirection direction );

    // Moves the snake part way!
    void partwaySnake( int x, int y, MoveDirection direction );

    inline int getRemainingFuel()
    {
        return fuelRemaining;
    }

    inline void setRemainingFuel( int f )
    {
        if ( f > getFuelMax() )
        {
            f = getFuelMax();
        }
        fuelRemaining = f;
    }

    inline int getStartingFuel()
    {
        return fuelStarting;
    }

    inline int getFuelMax()
    {
        return LEVEL_MAX_FUEL;
    }

    inline void setStartingFuel( int f )
    {
        fuelStarting = f;
    }

    // Marks a snake as used.
    void markSnakeUsed( int x, int y );

    // Animates level sprites.
    void animate();

    // Opens the exit(s).
    void openExit();

    // Elapsed time since start of level.
    int getElapsedTime();

private:

    // Get a level unit w/ wrap.
    LevelUnit getUnitWithWrap( int x, int y );

    // Computes the given trail char.
    char computeTrailChar( int x, int y, MoveDirection direction );

    // Puts a snake somewhere...
    void putSnakeAt( char trail, int x, int y );

    // Initialize variables.
    void init();

    // Stores the level data.
    Array2D<LevelUnit>* levelArray;

    // Snake units.
    Array2D<LevelPiece>* snakeArray;

    // Stores the sprite data.
    Array2D<Sprite*>* spriteArray;

    // Dimensions of our map, in terms
    // of pieces.
    int width;
    int height;

    // True if level is complete, else false.
    bool complete;

    // Number of castles.
    int numCastles;

    // Remaining castles.
    int remainingCastles;

    // Starting coordinates.
    int startX;
    int startY;

    // Fuel remaining at the start of the level.
    int fuelStarting;

    // Fuel remaining.
    int fuelRemaining;

    // Whether or not we left the starting position.
    bool leftStartingPos;

    std::set<Sprite*> spriteList;

    // Background sprite.
    Sprite* background;

    // Bottom area.
    Sprite* bottom;

    unsigned timer;

};

}

#endif /* LEVEL_H_ */
