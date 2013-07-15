#include "Utilities.h"
#include "Log.h"
#include "Config.h"
#include <sstream>
#include <algorithm>
#include <locale>
#include <cctype>
#include <cmath>
#include <math.h>

using namespace std;
using namespace Escarabajo;


string Utilities::longToString( long l )
{
	stringstream s;
	s << l;
	return s.str();
}

long Utilities::stringToLong( std::string s )
{
	return strtol( s.c_str(), NULL, 10 );
}

string Utilities::coordsToString( long x, long y )
{
	return "(" + longToString( x ) + ", " + longToString( y ) + ")";
}

bool Utilities::contains( std::string haystack, char needle )
{
	return haystack.find( needle ) != string::npos;
}

bool Utilities::contains( std::string haystack, std::string needle )
{
	return haystack.find( needle ) != string::npos;
}

vector<string> Utilities::parseCSV( string input )
{
	vector<string> ret;
	size_t comma = 0;
	size_t lastComma = -1;

	do
	{
		string s;
		comma = input.find_first_of( ',', lastComma + 1 );
		if ( comma != string::npos )
		{
			s = input.substr( lastComma + 1, comma - lastComma - 1);
		}
		else
		{
			s = input.substr( lastComma + 1 );
		}
		ret.push_back( s );
		lastComma = comma;
	}
	while ( string::npos != comma );

	return ret;
}

std::string Utilities::toLower( std::string s )
{
	string ret = "";
	std::transform( s.begin(),
	                s.end(),
	                std::back_inserter( ret ),
	                ::tolower );
	return ret;
}


std::string Utilities::toUpper( std::string s )
{
	string ret = "";
	std::transform( s.begin(),
	                s.end(),
	                std::back_inserter( ret ),
	                ::toupper );
	return ret;
}

std::string Utilities::removeLeadingSpaces( std::string s )
{
	string ret = "";
	size_t firstChar = s.find_first_not_of( " \n\r\t" );
	if ( firstChar != string::npos )
	{
		ret = s.substr( firstChar );
	}
	return ret;
}

std::string Utilities::removeLaggingSpaces( std::string s )
{
	string ret = "";
	size_t lastChar = s.find_last_not_of( " \n\r\t" );
	if ( lastChar != string::npos )
	{
		ret = s.substr( 0, lastChar + 1 );
	}
	return ret;
}

std::string Utilities::removeExtraSpaces( std::string s )
{
	s = removeLeadingSpaces( s );
	s = removeLaggingSpaces( s );
	return s;
}

std::vector<std::string> Utilities::stringToVector( std::string s, char delimiter )
{
	vector<string> ret;
	size_t start = 0;
	size_t finish = s.find_first_of( delimiter, start + 1 );
	while ( true )
	{
		string current = s.substr( start, finish - start );
		current = removeExtraSpaces( current );
		ret.push_back( current );
		start = s.find_first_of( delimiter, finish );
		if ( start == string::npos ) break;
		start++; // advance past the comma!
		finish = s.find_first_of( delimiter, start + 1 );
		if ( finish == string::npos ) finish = s.size();

	}
	return ret;
}

unsigned Utilities::nextPowerOf2( unsigned n )
{
	return (unsigned) pow( 2, ceil( log( n ) / log( 2 ) ) );
}


// From: http://www.gamedev.net/reference/programming/features/sdl2/page5.asp
Uint32 Utilities::GetPixel( SDL_Surface *surface, int x, int y )
{
	Uint32 col = 0 ;

	//determine position
	char* pPosition = ( char* ) surface->pixels ;

	//offset by y
	pPosition += ( surface->pitch * y ) ;

	//offset by x
	pPosition += ( surface->format->BytesPerPixel * x ) ;

	//copy pixel data
	memcpy ( &col , pPosition , surface->format->BytesPerPixel ) ;

	return ( col ) ;
}

// From: http://www.gamedev.net/reference/programming/features/sdl2/page5.asp
void Utilities::SetPixel( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	//determine position
	char* pPosition = ( char* ) surface->pixels ;

	//offset by y
	pPosition += ( surface->pitch * y ) ;

	//offset by x
	pPosition += ( surface->format->BytesPerPixel * x ) ;

	//copy pixel data
	memcpy ( pPosition , &pixel , surface->format->BytesPerPixel ) ;
}


GLuint Utilities::SDLSurfaceToOpenGLTexture( SDL_Surface* newSurface, int width, int height )
{
	GLuint newFrame = 0;

	// Load the image.
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &newFrame );
	glBindTexture(GL_TEXTURE_2D, newFrame);

	// Convert surface to Open GL format.
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4,
			width, height, GL_RGBA,GL_UNSIGNED_BYTE, newSurface->pixels);

	return newFrame;
}


// Sets a clipping rectangle.
void Utilities::clippingRectSet( int x, int y, int width, int height )
{
	int zoom = Config::getZoomFactor();
	int realWidth = width * zoom;
	int realHeight = height * zoom;

	int realX = x * zoom;
	int realY = Config::getScreenHeight() - (y * zoom) - realHeight;

	glEnable( GL_SCISSOR_TEST );
	glScissor( realX, realY, realWidth, realHeight );
}


// Unsets the clipping rectangle.
void Utilities::clippingRectUnset()
{
	glDisable( GL_SCISSOR_TEST );
}

