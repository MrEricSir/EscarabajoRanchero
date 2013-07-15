#include "Graphics.h"

using namespace Escarabajo;
using namespace std;



// Sets the size of the window.
void Graphics::setSize( int width, int height )
{
	screenWidth = width;
	screenHeight = height;
}


void Graphics::setWindowTitle( string t )
{
	this->title = t;
	SDL_WM_SetCaption( t.c_str(), NULL );
}


// Init the graphics.
void Graphics::init()
{
	const SDL_VideoInfo *pSDLVideoInfo = SDL_GetVideoInfo();

	if( !pSDLVideoInfo )
	{
		Log::write( "FATALITY! SDL error: " + string( SDL_GetError() ) );
		SDL_Quit();
		return;
	}

	int nFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE;

	// Check for hardware surfaces.
	if( pSDLVideoInfo->hw_available )
	{
		nFlags |= SDL_HWSURFACE;
	}
	else
	{
		nFlags |= SDL_SWSURFACE;
	}

	// Check for hardware blitting.
	if( pSDLVideoInfo->blit_hw )
	{
		nFlags |= SDL_HWACCEL;
	}

	// Enable double buffering.
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	// Create our rendering surface
	SDL_Surface *pSDLSurface = SDL_SetVideoMode( Config::getScreenWidth(), Config::getScreenHeight(), 32, nFlags );

	if( !pSDLSurface )
	{
		Log::write( "Call to SDL_SetVideoMode() failed! - SDL_Error: " + string( SDL_GetError() ) );
		SDL_Quit();
		return;
	}

	// Setup texture and drawing state.
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth( 1.0f );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_DEPTH_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glEnable(GL_CULL_FACE);

    // View size/shape.
    glViewport(0,0,screenWidth,screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,screenWidth,0,screenHeight,-100,100);
    glMatrixMode(GL_MODELVIEW);
}

// Destroy the graphics.
void Graphics::destroy()
{

}


// Start drawing our frame.
void Graphics::beginRendering()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

// Okay, we're done!
void Graphics::endRendering()
{
	SDL_GL_SwapBuffers();
}



