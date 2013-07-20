

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "all.h"

namespace Escarabajo
{

class Utilities
{

public:

    // Converts a long to a string.
    static std::string longToString( long l );

    // Converts a string to a long.
    static long stringToLong( std::string s );

    // Converts coordinates to an "(x, y)" string.
    static std::string coordsToString( long x, long y );

    // True if haystack contains needle.
    static bool contains( std::string haystack, char needle );

    // True if haystack contains needle.
    static bool contains( std::string haystack, std::string needle );

    // Converts a CSV to a string vector.
    static std::vector<std::string> parseCSV( std::string input );

    // Converts a string to lower case.
    static std::string toLower( std::string s );

    // Converts a string to upper case.
    static std::string toUpper( std::string s );

    // Removes spaces at end of a string.
    static std::string removeLeadingSpaces( std::string s );

    // Removes spaces at beginning of a string.
    static std::string removeLaggingSpaces( std::string s );

    // Removes spaces at beginning and end of a string.
    static std::string removeExtraSpaces( std::string s );

    // Converts a string to a vector given a character delimiter.
    // Leading and lagging spaces are removed from each string.
    static std::vector<std::string> stringToVector( std::string s, char delimiter );

    // Finds the next greatest power of 2.
    static unsigned nextPowerOf2( unsigned n );

    // Gets a pixel in Uint32 format.
    // Can use MapRGBA on it for individual values.
    static Uint32 GetPixel( SDL_Surface *surface, int x, int y );

    // Sets a pixel.
    static void SetPixel( SDL_Surface *surface, int x, int y, Uint32 pixel );

    // Converts an SDL surface to an OpenGL surface.
    static GLuint SDLSurfaceToOpenGLTexture( SDL_Surface* newSurface, int width, int height );

    // Sets a clipping rectangle.
    static void clippingRectSet( int x, int y, int width, int height );

    // Unsets the clipping rectangle.
    static void clippingRectUnset();

};

}

#endif /* UTILITIES_H_ */
