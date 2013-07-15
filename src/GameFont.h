#include "Sprite.h"
#include <string>
#include "SpriteMap.h"
#include "SpriteMapFactory.h"
#include "Config.h"

#ifndef GAMEFONT_H_
#define GAMEFONT_H_

namespace Escarabajo
{

/**
 * Represents a font used in the game.
 */
class GameFont
{
public:

	/** Loads a font from a map file.
	 */
	void load( std::string filename );

	/** Draws a character.
	 */
	void draw( int x, int y, char c );

	/** Draws a string.
	 */
	void draw( int x, int y, std::string c );

	/** Draws a character.
	 */
	void draw( int x, int y, int z, char c );

	/** Draws a string.
	 */
	void draw( int x, int y, int z, std::string c );

private:

	// Our sprite map.
	SpriteMap spriteMap;

	// Default z value.
	static const int defaultZ = 1;

	// Default font width.
	static const int fontWidth = 6;

};

}

#endif /* GAMEFONT_H_ */
