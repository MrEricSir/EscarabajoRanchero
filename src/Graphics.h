#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "all.h"
#include "GameObject.h"
#include "Log.h"

namespace Escarabajo
{

class Graphics
{
public:

    // Sets the size of the window.
    void setSize( int width, int height );

    // Init the graphics.
    void init();

    void setWindowTitle( std::string t );

    // Destroy the graphics.
    void destroy();

    // Draw frame.
    void beginRendering();
    void endRendering();

private:
    std::string title;
    int screenWidth;
    int screenHeight;

};

}


#endif /* GRAPHICS_H_ */
