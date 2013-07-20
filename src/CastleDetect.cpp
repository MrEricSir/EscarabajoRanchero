#include "CastleDetect.h"

using namespace Escarabajo;
using namespace std;

CastleDetect::~CastleDetect()
{
    // Delete our masks.
    delete mask;
    delete usedSnakes;
}

CastleDetect::CastleDetect( Level* l )
{
    level = l;
    maxArea = CASTLE_DETECT_DEFAULT_MAX_AREA;

    // Make our masks the same size as the level.
    mask = new Array2D<bool>( l->getWidth9(), l->getHeight9() );
    usedSnakes = new Array2D<bool>( l->getWidth9(), l->getHeight9() );
}

void CastleDetect::setMaxArea( int a )
{
    maxArea = a;
}

Array2D<bool>* CastleDetect::getMask()
{
    return mask;
}

void CastleDetect::mergeUsedSnakes()
{
    // Eliminate SNAKES!!!
    for ( int y = 0; y < usedSnakes->getHeight(); ++y )
    {
        for ( int x = 0; x < usedSnakes->getWidth(); ++x )
        {
            if ( usedSnakes->get( x, y ) )
            {
                level->markSnakeUsed( x, y );
            }

        }
    }
}

void CastleDetect::resetInternal()
{
    mask->setAll( false );
    numCastles = 0;
    bounded = true;
    numTiles = 0;
}

void CastleDetect::resetDetection()
{
    usedSnakes->setAll( false );
}


bool CastleDetect::runDetection( int x, int y )
{
    Log::write( "-----------------------" );
    Log::write( "Run detection " + Utilities::longToString( x ) +" , " + Utilities::longToString(  y  ) );

    // Show snakes!
//    for ( int y2 = 0; y2 < level->getHeight9(); ++y2 )
//        {
//            string line = "";
//            for ( int x2 = 0; x2 < level->getWidth9(); ++x2 )
//            {
//                line += Utilities::longToString(level->getPiece9(x2,y2) == SNAKE);
//
//            }
//            Log::write( line );
//    }

    resetInternal();
    detectRecursive( x, y );

    Log::write( "Number of tiles: " + Utilities::longToString( numTiles ) );
    Log::write( "Bounded:" + Utilities::longToString( bounded ) );
    Log::write( "Number of castles:" + Utilities::longToString( numCastles ) );

    // Check to see if we've found an acceptable area or not.
    if ( bounded && numCastles == 9 && numTiles <= maxArea )
    {
        Log::write("checking snakes!");
        if ( checkSnakes( x, y ) )
        {
            return true;
        }
    }
    return false;
}

bool CastleDetect::detectRecursive( int testX, int testY )
{
    // Stop conditions.

    if ( numTiles > maxArea )
    {
        bounded = false;
    }


    // Test rollover area.
    if ( testX < 0 )
    {
        testX = level->getWidth9() - 1;
    }
    else if ( testX >= level->getWidth9() )
    {
        testX = 0;
    }

    if     ( testY < 0 )
    {
        testY = level->getHeight9() - 1;
    }
    else if ( testY >= level->getHeight9() )
    {
        testY = 0;
    }

    if ( !bounded )
    {

        return false;
    }

    // Check area.
    if ( level->getPiece9( testX, testY ) == SNAKE ) return false;
    if ( mask->get( testX, testY ) == true ) return false;

    // See if the space is a castle.
    if ( level->getPiece9( testX, testY ) == CASTLE ||
            level->getPiece9( testX, testY ) == CASTLE1 ||
            level->getPiece9( testX, testY ) == CASTLE2 ||
            level->getPiece9( testX, testY ) == CASTLE3 ||
            level->getPiece9( testX, testY ) == CASTLE4 ||
            level->getPiece9( testX, testY ) == CASTLE5 ||
            level->getPiece9( testX, testY ) == CASTLE6 ||
            level->getPiece9( testX, testY ) == CASTLE7 ||
            level->getPiece9( testX, testY ) == CASTLE8 ||
            level->getPiece9( testX, testY ) == CASTLE9 ||
            level->getPiece9( testX, testY ) == CASTLE_CRUMBLE )
    {
        numCastles++;
    }

    // Mark our territory (so we don't repeat!)
    mask->set( true, testX, testY );
    ++numTiles;

    // Recurse to every direction.
    return
        detectRecursive( testX, testY - 1 ) ||
        detectRecursive( testX, testY + 1 ) ||
        detectRecursive( testX - 1, testY ) ||
        detectRecursive( testX + 1, testY );
}


bool CastleDetect::checkSnakes( int x, int y )
{
    Array2D<int> edgeMask( level->getWidth9(), level->getHeight9() );
    edgeMask.setAll( 0 );

    Log::write( "CheckSnakes: set all " );

    // Create our edge mask.
    checkSnakeRecursive( x, y, edgeMask );

    Log::write( "returned from recursive" );

    bool ret = true;

    // Make sure it's all SNAKES!!!
    for ( int y = 0; y < edgeMask.getHeight(); ++y )
    {
        string line = "";
        for ( int x = 0; x < edgeMask.getWidth(); ++x )
        {
            line += Utilities::longToString(mask->get(x,y));
            if ( edgeMask.get( x, y ) == 2 )
            {
                if ( level->getPiece9( x, y ) != SNAKE )
                {
                    //return false;
                    ret = false;
                    Log::write( Utilities::longToString(level->getPiece9( x, y ) ) );
                }
            }
        }
        //Log::write( line );
    }

    return ret;

    // Mark snake areas, create mask.
//    for ( int y = 0; y < edgeMask.getHeight(); ++y )
//    {
//        for ( int x = 0; x < edgeMask.getWidth(); ++x )
//        {
//            if ( edgeMask.get( x, y ) == 2 )
//            {
//                usedSnakes->set( true, x, y );
//                mask->set( true, x, y );
//            }
//        }
//    }

    return true;
}

void CastleDetect::checkSnakeRecursive( int testX, int testY, Array2D<int>& edgeMask )
{
    // Test rollover area
    if ( ( testX < 0 || testX >= level->getWidth9() ) ||
        ( testY < 0 || testY >= level->getHeight9() ) )
    {
        // TODO: should be handled as in detectRecursive
        return;
    }

    if ( edgeMask.get( testX, testY ) == 1 ) return;

    // Outside mask, mark it!.
    if ( !mask->get( testX, testY ) )
    {
        edgeMask.set( 2, testX, testY );
        return;
    }

    edgeMask.set( 1, testX, testY );

    // Top three.
    checkSnakeRecursive( testX - 1, testY - 1, edgeMask );
    checkSnakeRecursive( testX,     testY - 1, edgeMask );
    checkSnakeRecursive( testX + 1, testY - 1, edgeMask );

    // Left and right.
    checkSnakeRecursive( testX - 1, testY, edgeMask );
    checkSnakeRecursive( testX + 1, testY, edgeMask );

    // Top three.
    checkSnakeRecursive( testX - 1, testY + 1, edgeMask );
    checkSnakeRecursive( testX,     testY + 1, edgeMask );
    checkSnakeRecursive( testX + 1, testY + 1, edgeMask );
}


