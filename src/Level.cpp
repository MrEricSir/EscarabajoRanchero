#include "Level.h"
#include "Utilities.h"

using namespace Escarabajo;
using namespace std;

void Level::init()
{
	width = 0;
	height = 0;
	levelArray = NULL;
	spriteArray = NULL;

	complete = false;

	fuelRemaining = fuelStarting;

	numCastles = 0;
	remainingCastles = 0;

	leftStartingPos = false;

	background = NULL;

	timer = 0;
}

Level::Level()
{
	init();
}

Level::Level( int width, int height )
{
	init();

	this->width = width;
	this->height = height;

	// Allocate level array.
	levelArray = new Array2D<LevelUnit>( width, height );
	LevelUnit lu;
	levelArray->setAll( lu );

	// Allocate sprite array.
	spriteArray = new Array2D<Sprite*>( width, height );
	spriteArray->setAll( NULL );

	// Allocate silk trail unit array.
	snakeArray = new Array2D<LevelPiece>( width * 3, height * 3 );
	snakeArray->setAll( EMPTY );

	// Bottom sprite.
	bottom = new Sprite();
	bottom->setSolidColor( width, height, 0, 0, 0, 255 );
}


Level::~Level()
{
	if ( NULL != levelArray ) delete levelArray;
	if ( NULL != spriteArray ) delete spriteArray;
	if ( NULL != snakeArray ) delete snakeArray;
	if ( NULL != bottom ) delete bottom;
}


void Level::setBackgroundSprite( Sprite* s )
{
	this->background = s;
}


void Level::setPiece( const LevelPiece& p, const int& x, const int& y )
{
//	if ( p != SNAKE )
//	{
//		Log::write( "Level: setPiece " + Utilities::longToString( p ) + " at " +
//			Utilities::coordsToString( x, y ) );
//	}

	LevelUnit lu = levelArray->get( x, y );
	lu.piece = p;
	levelArray->set( lu, x, y );
}

LevelPiece Level::getPiece( const int& x, const int& y )
{
	LevelPiece test = snakeArray->get( x * 3 + 1, y * 3 + 1 );
	if ( test != EMPTY )
	{
		return test;
	}

	return levelArray->get( x, y ).piece;
}


LevelUnit Level::getUnitWithWrap( int x, int y )
{
	if ( x >= width )
	{
		x = x - width;
	}
	else if ( x < 0 )
	{
		x = x + width;
	}

	if ( y >= height )
	{
		y = y - height;
	}
	else if ( y < 0 )
	{
		y = y + height;
	}

	return levelArray->get( x, y );
}

LevelPiece Level::getPieceWithWrap( int x, int y )
{
	return getUnitWithWrap( x, y ).piece;
}

void Level::setAcceptDirections( int x, int y, bool acceptTop, bool acceptBottom, bool acceptRight, bool acceptLeft )
{
	LevelUnit lu = levelArray->get( x, y );
	lu.acceptTop 		= acceptTop;
	lu.acceptBottom 	= acceptBottom;
	lu.acceptRight 	= acceptRight;
	lu.acceptLeft 	= acceptLeft;

	levelArray->set( lu, x, y );
}

void Level::setSprite( Sprite* s, const int& x, const int& y )
{
	spriteArray->set( s, x, y );
	if ( s != NULL )
	{
		spriteList.insert( s );
	}
}

void Level::draw()
{
	// Draw tessellated background sprite.
	if ( background )
	{
		int maxX = Config::getScreenWidth();
		int maxY = Config::getScreenHeight();

		for ( int y = 0; y < maxY; y+= background->getHeight() )
		{
			for ( int x = 0; x < maxX; x+= background->getWidth() )
			{
				background->draw( x, y, -10 );
			}
		}
	}

	// Black out area under scorebox.
	int yBottom = height * Config::getHeightOfLevelPiece();
	for ( int x = 0; x < width; x++ )
	{
		bottom->draw( x * Config::getWidthOfLevelPiece(), yBottom, 10 );
	}

	// Draw level sprites.
	for ( int y = 0; y < height; y++ )
	{
		for ( int x = 0; x < width; x++ )
		{
			Sprite* s = spriteArray->get(x, y);
			if ( NULL != s )
			{
				// Could optimize this slightly, if need be.
				s->draw( x * Config::getWidthOfLevelPiece(), y * Config::getHeightOfLevelPiece(), -5 );
			}
		}
	}

}


int Level::getWidth()
{
	return width;
}

int Level::getHeight()
{
	return height;
}

/**
 * Set the starting position.
 */
void Level::setStartingPosition( const int& x, const int& y )
{
	startX = x;
	startY = y;
}

int Level::getStartingX()
{
	return startX;
}

int Level::getStartingY()
{
	return startY;
}


bool Level::canMoveTo( int x, int y, MoveDirection dir )
{
	LevelUnit lu = getUnitWithWrap( x, y );
	if
	(
		( dir == MOVE_UP 	&& lu.acceptBottom ) ||
		( dir == MOVE_DOWN	&& lu.acceptTop    ) ||
		( dir == MOVE_RIGHT && lu.acceptLeft   ) ||
		( dir == MOVE_LEFT 	&& lu.acceptRight  )
	)
	{
		return true;
	}
	return false;
}


Level* Level::clone()
{
	Level* ret = new Level( width, height );

	// Copy level array.
	{
		for ( int y = 0; y < height; ++y )
		{
			for ( int x = 0; x < width; ++x )
			{
				ret->levelArray->set( levelArray->get( x, y ), x ,y );
			}
		}
	}

	// Copy sprite array.
	{
		for ( int y = 0; y < height; ++y )
		{
			for ( int x = 0; x < width; ++x )
			{
				ret->spriteArray->set( spriteArray->get( x, y ), x ,y );
			}
		}
	}

	// Copy sprite list.
	{
		std::set<Sprite*>::iterator it;
		for ( it = spriteList.begin(); it != spriteList.end(); ++it )
		{
			ret->spriteList.insert( *it );
		}
	}

	// Number of castles.
	ret->numCastles = numCastles;

	// Remaining castles.
	ret->remainingCastles = remainingCastles;

	// Starting coordinates.
	ret->startX = startX;
	ret->startY = startY;

	// Fuel remaining at the start of the level.
	ret->fuelStarting = fuelStarting;

	// Fuel remaining (same as fuel starting)
	ret->fuelRemaining = fuelStarting;

	// Background sprite.
	ret->background = background;

	return ret;
}


void Level::setComplete( bool c )
{
	complete = c;
}


void Level::putSnakeAt( char trail, int x, int y )
{
	// Draw the char.
	Sprite* s = LevelFactory::getSprite( trail );
	setSprite( s, x, y );

	// Set the accept directions.
	if ( TRAIL_HORIZONTAL == trail )
	{
		setAcceptDirections( x, y, true, true, false, false );
	}
	else if ( TRAIL_VERTICAL == trail )
	{
		setAcceptDirections( x, y, false, false, true, true );
	}
	else
	{
		setAcceptDirections( x, y, false, false, false, false );
	}

	// Set sprite char.
	LevelUnit lu = levelArray->get( x, y );
	lu.trailChar = trail;
	levelArray->set( lu, x, y );

	// Set snake array char.
	int sx = x * 3;
	int sy = y * 3;

	for ( int j = 0; j <= 2; ++j )
	{
		for ( int i = 0; i <= 2; ++i )
		{
			snakeArray->set( EMPTY, sx + i, sy + j );
		}
	}

	switch ( trail )
	{
		case TRAIL_CROSS :
			snakeArray->set( SNAKE, sx + 1, sy + 0 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 1, sy + 2 );

			snakeArray->set( SNAKE, sx + 0, sy + 1 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 2, sy + 1 );
			break;
		case TRAIL_HORIZONTAL :
			snakeArray->set( SNAKE, sx + 0, sy + 1 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 2, sy + 1 );
			break;
		case TRAIL_VERTICAL :
			snakeArray->set( SNAKE, sx + 1, sy + 0 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 1, sy + 2 );
			break;
		case TRAIL_CORNER_UP_RIGHT :
			snakeArray->set( SNAKE, sx + 1, sy + 0 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 2, sy + 1 );
			break;
		case TRAIL_CORNER_UP_LEFT :
			snakeArray->set( SNAKE, sx + 1, sy + 0 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 0, sy + 1 );
			break;
		case TRAIL_CORNER_DOWN_RIGHT :
			snakeArray->set( SNAKE, sx + 1, sy + 2 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 2, sy + 1 );
			break;
		case TRAIL_CORNER_DOWN_LEFT :
			snakeArray->set( SNAKE, sx + 1, sy + 2 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 0, sy + 1 );
			break;

		case TRAIL_TRI_UP :
			snakeArray->set( SNAKE, sx + 1, sy + 0 );
			snakeArray->set( SNAKE, sx + 0, sy + 1 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 2, sy + 1 );
			break;
		case TRAIL_TRI_DOWN :
			snakeArray->set( SNAKE, sx + 1, sy + 2 );
			snakeArray->set( SNAKE, sx + 0, sy + 1 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 2, sy + 1 );
			break;
		case TRAIL_TRI_LEFT :
			snakeArray->set( SNAKE, sx + 0, sy + 1 );
			snakeArray->set( SNAKE, sx + 1, sy + 0 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 1, sy + 2 );
			break;
		case TRAIL_TRI_RIGHT :
			snakeArray->set( SNAKE, sx + 2, sy + 1 );
			snakeArray->set( SNAKE, sx + 1, sy + 0 );
			snakeArray->set( SNAKE, sx + 1, sy + 1 );
			snakeArray->set( SNAKE, sx + 1, sy + 2 );
			break;
//		case TRAIL_START_UP :
//			break;
//		case TRAIL_START_DOWN :
//			break;
//		case TRAIL_START_LEFT :
//			break;
//		case TRAIL_START_RIGHT :
//			break;
		default:
			// No sir, I don't like it.
			break;

	}


}


char Level::computeTrailChar( int x, int y, MoveDirection direction )
{
	char unitAbove = getUnitWithWrap( x, y - 1 ).trailChar;
	char unitBelow = getUnitWithWrap( x, y + 1 ).trailChar;
	char unitRight = getUnitWithWrap( x + 1, y ).trailChar;
	char unitLeft =  getUnitWithWrap( x - 1, y ).trailChar;

	char trailChar = TRAIL_CROSS; // default

	// Figure out which other directions we can connect to.
	bool above =  ( unitAbove == TRAIL_CROSS ||
					unitAbove == TRAIL_VERTICAL ||
					unitAbove == TRAIL_CORNER_DOWN_RIGHT ||
					unitAbove == TRAIL_CORNER_DOWN_LEFT ||
					unitAbove == TRAIL_TRI_DOWN ||
					unitAbove == TRAIL_TRI_RIGHT ||
					unitAbove == TRAIL_TRI_LEFT );
	bool below =  ( unitBelow == TRAIL_CROSS ||
					unitBelow == TRAIL_VERTICAL ||
					unitBelow == TRAIL_CORNER_UP_RIGHT ||
					unitBelow == TRAIL_CORNER_UP_LEFT ||
					unitBelow == TRAIL_TRI_UP ||
					unitBelow == TRAIL_TRI_LEFT ||
					unitBelow == TRAIL_TRI_RIGHT );
	bool right =  ( unitRight == TRAIL_CROSS ||
					unitRight == TRAIL_HORIZONTAL ||
					unitRight == TRAIL_CORNER_UP_LEFT ||
					unitRight == TRAIL_CORNER_DOWN_LEFT ||
					unitRight == TRAIL_TRI_LEFT ||
					unitRight == TRAIL_TRI_UP ||
					unitRight == TRAIL_TRI_DOWN );
	bool left  =  ( unitLeft == TRAIL_CROSS ||
					unitLeft == TRAIL_HORIZONTAL ||
					unitLeft == TRAIL_CORNER_UP_RIGHT ||
					unitLeft == TRAIL_CORNER_DOWN_RIGHT ||
					unitLeft == TRAIL_TRI_RIGHT ||
					unitLeft == TRAIL_TRI_UP ||
					unitLeft == TRAIL_TRI_DOWN );

	// Now figure out the direction!
	if ( getStartingX() == x && getStartingY() == y && leftStartingPos )
	{
		trailChar = TRAIL_CROSS;
	}
	else if ( above && below && right && left )
	{
		trailChar = TRAIL_CROSS;
	}
	else if ( above && below && right && direction == MOVE_LEFT )
	{
		trailChar = TRAIL_CROSS;
	}
	else if ( above && below && left && direction == MOVE_RIGHT )
	{
		trailChar = TRAIL_CROSS;
	}
	else if ( right && left && above && direction == MOVE_DOWN )
	{
		trailChar = TRAIL_CROSS;
	}
	else if ( right && left && below && direction == MOVE_UP )
	{
		trailChar = TRAIL_CROSS;
	}
	else if ( right && direction == MOVE_UP )
	{
		if ( below )
		{
			trailChar = TRAIL_TRI_RIGHT;
		}
		else
		{
			trailChar = TRAIL_CORNER_UP_RIGHT;
		}
	}
	else if ( above && direction == MOVE_RIGHT )
	{
		if ( left )
		{
			trailChar = TRAIL_TRI_UP;
		}
		else
		{
			trailChar = TRAIL_CORNER_UP_RIGHT;
		}
	}
	else if ( right && direction == MOVE_DOWN )
	{
		if ( above )
		{
			trailChar = TRAIL_TRI_RIGHT;
		}
		else
		{
			trailChar = TRAIL_CORNER_DOWN_RIGHT;
		}
	}
	else if ( below && direction == MOVE_RIGHT )
	{
		if ( above )
		{
			trailChar = TRAIL_TRI_RIGHT;
		}
		else
		{
			trailChar = TRAIL_CORNER_DOWN_RIGHT;
		}
	}
	else if ( left && direction == MOVE_UP )
	{
		if ( below )
		{
			trailChar = TRAIL_TRI_LEFT;
		}
		else
		{
			trailChar = TRAIL_CORNER_UP_LEFT;
		}
	}
	else if ( above && direction == MOVE_LEFT )
	{
		if ( right )
		{
			trailChar = TRAIL_TRI_UP;
		}
		else
		{
			trailChar = TRAIL_CORNER_UP_LEFT;
		}
	}
	else if ( left && direction == MOVE_DOWN )
	{
		if ( above )
		{
			trailChar = TRAIL_TRI_LEFT;
		}
		else
		{
			trailChar = TRAIL_CORNER_DOWN_LEFT;
		}
	}
	else if  ( below && direction == MOVE_LEFT )
	{
		if ( above )
		{
			trailChar = TRAIL_TRI_LEFT;
		}
		else
		{
			trailChar = TRAIL_CORNER_DOWN_LEFT;
		}
	}
	else if ( direction == MOVE_UP || direction == MOVE_DOWN )
	{
		trailChar = TRAIL_VERTICAL;
	}
	else if ( direction == MOVE_LEFT || direction == MOVE_RIGHT )
	{
		trailChar = TRAIL_HORIZONTAL;
	}

	if ( !leftStartingPos && ( x != getStartingX() || y != getStartingY() ) )
	{
		leftStartingPos = true;
	}

	return trailChar;
}


void Level::drawSnake( int x, int y, MoveDirection direction )
{
	char trailChar = computeTrailChar( x, y, direction );

	// Take care of it, charlie.
	// ...wait, who the hell is charlie?!?!
	putSnakeAt( trailChar, x, y );
}


void Level::markSnakeUsed( int x, int y )
{
	if ( snakeArray->get( x, y ) == SNAKE )
	{
		snakeArray->set( SNAKEUSED, x, y );
	}
}


void Level::animate()
{
	// Update animation.
	std::set<Sprite*>::iterator it;
	for ( it = spriteList.begin(); it != spriteList.end(); ++it )
	{
		(*it)->animAdvance();
	}

	// Update timer.
	static Uint32 lastTime = 0;

	if ( timer < 999999 )
	{
		Uint32 now = SDL_GetTicks();
		if ( 0 == lastTime )
		{
			lastTime = now;
		}
		else
		{
			timer += ( now - lastTime );
			lastTime = now;
			if ( timer > 999999 )
			{
				timer = 999999;
			}
		}
	}
}


int Level::getElapsedTime()
{
	return timer / 1000;
}

void Level::openExit()
{

	for ( int y = 0; y < height; ++y )
	{
		for ( int x = 0; x < width; ++x )
		{
			if ( levelArray->get( x, y ).piece == FINISH )
			{
				setAcceptDirections( x, y, true, true, true, true );
			}
		}
	}

}


