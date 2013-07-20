#include "all.h"
#include "Sprite.h"
#include "MoveDirection.h"
#include <map>

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

namespace Escarabajo
{

class Sprite;

class GameObject
{

public:

    // Ctor.
    GameObject();

    // Sets the sprite for the current direction.
    void setSprite( Sprite* s );

    // Sets the sprite for a given direction.
    void setSprite( MoveDirection d, Sprite* s );

    // Changes the direction of the objection.
    void setDirection( MoveDirection d );

    Sprite* getSprite();
    void setWidth( int w );
    void setHeight( int h );
    void setX( int x );
    void setY( int y );
    int getX( void );
    int getY( void );
    void setLocation( int x, int y );

    // Animation!
    void animateObject();
    void animateMotion();
    void setAnimOffset( int x, int y );
    int getAnimOffsetX();
    int getAnimOffsetY();

    // Draws the object.
    void draw();

private:
    int x;
    int y;
    int width;
    int height;

    int animOffsetX;
    int animOffsetY;

    int animStep;

    MoveDirection direction;

    // Maps directions to sprite.
    std::map<MoveDirection, Sprite*> directionMap;

    bool skipFrame;

};

}

#endif /* GAMEOBJECT_H_ */
