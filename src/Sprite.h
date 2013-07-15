#ifndef SPRITE_H_
#define SPRITE_H_

#include <vector>
#include "all.h"
#include "Config.h"
#include "Log.h"

namespace Escarabajo
{

/**
 * Represents a single graphic.
 */
class Sprite
{
public:

	Sprite();

	// Loads a sprite from an image file.
	// Will load to the index (starting at zero) in the animation sequence.
	void loadImage( std::string filename );

	// Loads a vector of image filenames into the animation sequence.
	void loadImageSequence( std::vector< std::string> filenames );

	// Creates a one color, solid sprite.
	void setSolidColor( int width, int height, int r, int g, int b, int a );

	// Draws the sprite at a given pixel location.
	void draw( float x, float y );
	void draw( float x, float y, float z );

	// Gets the width of the sprite.
	int getWidth();

	// Gets the height of the sprite.
	int getHeight();

	// Advances the animation to the next frame, if there is one.
	void animAdvance();

	// Returns the number of frames in the animation.
	int animSize();

	// Resets the animation.
	void animReset();

private:

	// Internal function for setting dimensions.
	void setDimensions( int width, int height );

	// List of textures.
	std::vector< GLuint > textureList;

	// Width.
	GLuint width;

	// Height.
	GLuint height;

	// Dimensions set to nearest power of 2.
	GLuint widthPow;
	GLuint heightPow;

	// Whether or not animation loops.
	bool loop;

	// Current animation frame.
	int currentFrame;

	struct Vertex
	{
	    float tu, tv;
	    float x, y, z;
	};

};

}

#endif /* SPRITE_H_ */
