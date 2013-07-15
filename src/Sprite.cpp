#include "Sprite.h"
#include "Utilities.h"

using namespace Escarabajo;
using namespace std;

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
   static const Uint32 rmask = 0x000000FF;
   static const Uint32 bmask = 0x0000FF00;
   static const Uint32 gmask = 0x00FF0000;
   static const Uint32 amask = 0xFF000000;
#else
   static const Uint32 rmask = 0xFF000000;
   static const Uint32 bmask = 0x00FF0000;
   static const Uint32 gmask = 0x0000FF00;
   static const Uint32 amask = 0x000000FF;
#endif


Sprite::Sprite()
{
	loop = true;
	currentFrame = 0;
}


void Sprite::draw( float x, float y )
{
	draw( x, y, 0 );
}

void Sprite::draw( float x, float y, float z )
{
	if ( textureList.size() ==0 ) return;
	GLuint frame = textureList[currentFrame];

	// Calculate location.
	float zoom = Config::getZoomFactor();
	float height = this->height * zoom;
	float width = this->width * zoom;

	float heightP = this->heightPow * zoom;
	float widthP = this->widthPow * zoom;

	glPushMatrix();
	float realX = x * zoom;
	float realY = Config::getScreenHeight() - (y * zoom) - heightP;
	glTranslatef( realX, realY, 0.0f );

	// Texture (no filtering.)
	glBindTexture( GL_TEXTURE_2D, frame );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Draw our shape.
	glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3i( 0,     0,      z );
		glTexCoord2f( 1.0f, 1.0f ); glVertex3i( widthP, 0,      z );
		glTexCoord2f( 1.0f, 0.0f ); glVertex3i( widthP, heightP, z );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3i( 0,     heightP, z );
    glEnd();

	glPopMatrix();
}


int Sprite::getWidth()
{
	return width;
}


int Sprite::getHeight()
{
	return height;
}


void Sprite::setDimensions( int width, int height )
{
	this->width = width;
	 this->height = height;

	 // Dimensions rounded up to nearest power of 2.
	 widthPow = Utilities::nextPowerOf2( width );
	 heightPow = Utilities::nextPowerOf2( height );
}


void Sprite::setSolidColor( int width, int height, int r, int g, int b, int a )
{
	 setDimensions( width, height );

	 // Create new surface.
	 SDL_Surface* newSurface = SDL_CreateRGBSurface( SDL_SRCALPHA,
	 		   widthPow, heightPow, 32,
	            rmask, bmask, gmask, amask );

	// Fill the entire rectangle with a solid alpha color.
	Uint32 alpha = 0;
	alpha = SDL_MapRGBA( newSurface->format, 0, 0, 0, amask );

	SDL_Rect rectFill;
	rectFill.x = 0;
	rectFill.y = 0;
	rectFill.h = heightPow;
	rectFill.w = widthPow;
	int ret = SDL_FillRect( newSurface, &rectFill, alpha);

	// Fill the part of the rectangle we care about with
	// the requested color.
	Uint32 color = 0;
	color = SDL_MapRGBA( newSurface->format, r, g, b, a );

	SDL_Rect rectColor;
	rectColor.x = 0;
	rectColor.y = 0;
	rectColor.h = height;
	rectColor.w = width;
	ret = SDL_FillRect( newSurface, &rectColor, color);

	// Create OGL texture and push it to our list.
	GLuint newFrame = Utilities::SDLSurfaceToOpenGLTexture( newSurface, widthPow, heightPow );
	textureList.push_back( newFrame );

	SDL_FreeSurface( newSurface );
}


void Sprite::loadImage( string filename )
{
	// Some of this was lifted from: http://ubuntuforums.org/showthread.php?t=396281
   SDL_Surface* surface = IMG_Load( filename.c_str() );

   if ( NULL == surface )
   {
	   Log::write( string("Sprite.loadImage: couldn't load sprite: ") + IMG_GetError() );
	   return;
   }


   SDL_PixelFormat *format = surface->format;

   // Remember dimensions.
   setDimensions( surface->w, surface->h );

   // Create new surface with empty edges.
   SDL_Surface* newSurface = SDL_CreateRGBSurface( SDL_SRCALPHA,
		   widthPow, heightPow, 32,
           rmask, bmask, gmask, amask );

   // Fill sprite with alpha.
   Uint32 alpha = 0;
   alpha = SDL_MapRGBA( format, 0, 0, 0, amask );
   SDL_Rect rect;
   rect.x = 0;
   rect.y = 0;
   rect.h = heightPow;
   rect.w = widthPow;
   int ret = SDL_FillRect( newSurface, &rect, alpha);
   surface->flags &= !SDL_SRCALPHA;


   //SDL_SetColorKey( newSurface, SDL_SRCCOLORKEY, format->colorkey );
   SDL_SetAlpha( newSurface, SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT );

   // Copy image data to our new surface.
   ret = SDL_BlitSurface( surface, 0, newSurface, 0 );


   // Change color key to alpha transparency.
   // Used for 24-bit images.
   if ( 24 == format->BitsPerPixel )
   {
	   Uint32 colorKey = format->colorkey;

	   for ( int y = 0; y < surface->h; ++y )
	   {
		   for (int x = 0; x < surface->w; x++)
		   {
			   Uint32 color = Utilities::GetPixel( surface, x, y );
			   if ( color == colorKey )
			   {
				   Utilities::SetPixel( newSurface, x, y, 0 );
			   }
		   }
	   }
   }

   // Convert surface to Open GL format.
   GLuint newFrame = Utilities::SDLSurfaceToOpenGLTexture( newSurface, widthPow, heightPow );

   // Add to our texture list.
   textureList.push_back( newFrame );

   // Free our temporary SDL buffers.
   SDL_FreeSurface( surface );
   SDL_FreeSurface( newSurface );
}


void Sprite::loadImageSequence( std::vector< std::string> filenames )
{
	if ( filenames.size() == 0 )
	{
		Log::write( "ERROR --- loadImageSequence got a vector of size 0" );
	}

	for ( unsigned i = 0; i < filenames.size(); ++i )
	{
		string file = filenames[i];
		if ( filenames.size() - 1 == i && "" == file )
		{
			loop = false;
		}
		else
		{
			loadImage( file );
		}
	}
}


// Advances the animation to the next frame, if there is one.
void Sprite::animAdvance()
{
	int max = animSize();
	if ( 0 == max || 1 == max )
	{
		return;
	}

	currentFrame++;
	//Log::write( "current frame = " + Utilities::longToString( currentFrame ) + " max = "+ Utilities::longToString( max ) );
	if ( currentFrame >= max )
	{
		if ( loop )
		{
			currentFrame = 0;
		}
		else
		{
			currentFrame = max - 1;
		}
	}

}


// Returns the number of frames in the animation.
int Sprite::animSize()
{
	return textureList.size();
}


// Resets the animation.
void Sprite::animReset()
{
	currentFrame = 0;
}



