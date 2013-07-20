#include "Dispatcher.h"

using namespace Escarabajo;


// Timer callback.
Uint32 DispatcherCallback(Uint32 interval, void *param);


Dispatcher::Dispatcher()
{
    myHandler = NULL;
    timerID = 0;
}


Dispatcher::~Dispatcher()
{
    unsetHandler();
}


void Dispatcher::setDispatchHandler( DispatchHandler* dh )
{
    unsetHandler();
    myHandler = dh;
    timerID = SDL_AddTimer( myHandler->getInterval(), DispatcherCallback, 0 );
}


void Dispatcher::unsetHandler()
{
    myHandler = NULL;
    SDL_RemoveTimer( timerID );
}


void Dispatcher::mainLoop()
{
    SDL_Event Event;
    while( SDL_WaitEvent( &Event ) != 0 )
    {
        if ( !myHandler )
        {
            continue;
        }

        // Timer.
        if ( SDL_USEREVENT == Event.type )
        {
            myHandler->tick();
            continue;
        }

        // Quit.
        if ( SDL_QUIT == Event.type )
        {
            if ( !myHandler->quit() )
            {
                return; // Exit loop.
            }
            continue;
        }

        // Key release.
        if ( SDL_KEYUP == Event.type )
        {
            switch ( Event.key.keysym.sym )
            {
            case SDLK_RIGHT:
                myHandler->releaseRight();
                continue;
            case SDLK_LEFT:
                myHandler->releaseLeft();
                continue;
            case SDLK_DOWN:
                myHandler->releaseDown();
                continue;
            case SDLK_UP:
                myHandler->releaseUp();
                continue;
            default:
                continue;
            }
        }

        // Key down.
        if ( SDL_KEYDOWN == Event.type )
        {
            switch ( Event.key.keysym.sym )
            {
            case SDLK_ESCAPE:
                myHandler->pressEscape();
                continue;
            case SDLK_p:
                myHandler->pressP();
                continue;
            case SDLK_r:
                myHandler->pressR();
                continue;
            case SDLK_PLUS:
            case SDLK_KP_PLUS:
                myHandler->pressPlus();
                continue;
            case SDLK_MINUS:
            case SDLK_KP_MINUS:
                myHandler->pressMinus();
                continue;
            case SDLK_RIGHT:
                myHandler->pressRight();
                continue;
            case SDLK_DOWN:
                myHandler->pressDown();
                continue;
            case SDLK_LEFT:
                myHandler->pressLeft();
                continue;
            case SDLK_UP:
                myHandler->pressUp();
                continue;
            default:
                continue;
            }
        }

    }

}


// Callback used for timing event.
Uint32 DispatcherCallback( Uint32 interval, void *param )
{
    SDL_Event event;
    event.type = SDL_USEREVENT;

    SDL_PushEvent(&event);
    return interval;
}




