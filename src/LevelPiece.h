#ifndef LEVELPIECE_H_
#define LEVELPIECE_H_

/**
 * Represents a piece (or square) of the level.
 */
enum LevelPiece
{
    EMPTY,       // Empty tile
    SNAKE,       // Snake piece
    SNAKEUSED,     // Used snake
    START,       // Start
    FINISH,        // Finish
    CASTLE,
    CASTLE1,     // ordered castle
    CASTLE2,     // ordered castle
    CASTLE3,     // ordered castle
    CASTLE4,     // ordered castle
    CASTLE5,     // ordered castle
    CASTLE6,     // ordered castle
    CASTLE7,     // ordered castle
    CASTLE8,     // ordered castle
    CASTLE9,     // ordered castle
    CASTLE_CRUMBLE, //  Crumbly castle
    CASTLE_CRUMBLE_WALL, // Walls for a crumbly castle
    WALL         // Wall
};

#endif /* LEVELPIECE_H_ */
