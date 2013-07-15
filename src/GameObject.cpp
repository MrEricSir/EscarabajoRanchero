#include "GameObject.h"

using namespace std;
using namespace Escarabajo;

GameObject::GameObject()
{
	direction = MOVE_UP;
	animOffsetX = 0;
	animOffsetY = 0;
	animStep = Config::getWidthOfLevelPiece() / Config::getAnimationStep();
	skipFrame = true;
}

void GameObject::setSprite( Sprite* s )
{
	directionMap[ direction ] = s;
}

void GameObject::setSprite( MoveDirection d, Sprite* s )
{
	directionMap[ d ] = s;
}

void GameObject::setDirection( MoveDirection d )
{
	direction = d;
}

Sprite* GameObject::getSprite()
{
	return directionMap[ direction ];
}

void GameObject::setWidth( int w )
{
	this->width = w;
}

void GameObject::setHeight( int h )
{
	this->height = h;
}

void GameObject::setX( int x )
{
	this->x = x;
	animOffsetX = 0;
	animOffsetY = 0;
}

void GameObject::setY( int y )
{
	this->y = y;
	animOffsetX = 0;
	animOffsetY = 0;
}

int GameObject::getX( void )
{
	return this->x;
}

int GameObject::getY( void )
{
	return this->y;
}

void GameObject::setLocation( int x, int y )
{
	this->x = x;
	this->y = y;
	animOffsetX = 0;
	animOffsetY = 0;
}

void GameObject::draw()
{
	getSprite()->draw( x * Config::getWidthOfLevelPiece() + animOffsetX,
			y * Config::getHeightOfLevelPiece() + animOffsetY );
}

void GameObject::animateObject()
{
	// advance frame
	getSprite()->animAdvance();
	skipFrame = !skipFrame;
}

void GameObject::animateMotion()
{
	// do one step
	switch ( direction )
	{
	case NONE:
		break;
	case MOVE_UP:
		animOffsetY -= animStep;
		break;
	case MOVE_DOWN:
		animOffsetY += animStep;
		break;
	case MOVE_LEFT:
		animOffsetX -= animStep;
		break;
	case MOVE_RIGHT:
		animOffsetX += animStep;
		break;
	default:
		break;
	}
}

void GameObject::setAnimOffset( int x, int y )
{
	animOffsetX = x;
	animOffsetY = y;
}

int GameObject::getAnimOffsetX()
{
	return animOffsetX;
}

int GameObject::getAnimOffsetY()
{
	return animOffsetY;
}


