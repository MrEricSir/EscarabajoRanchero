#include "Dispatcher.h"

using namespace Escarabajo;


// Timer callback.
Uint32 DispatcherCallback(Uint32 interval, void *param);


Dispatcher::Dispatcher() :
    timerID(0),
    eventHandler(NULL),
    keyHandler(NULL)
{
}


Dispatcher::~Dispatcher()
{
    setEventHandler(NULL);
    setKeyHandler(NULL);
}


void Dispatcher::setEventHandler( DispatchEventHandler* dh )
{
    // Unset.
    eventHandler = NULL;
    SDL_RemoveTimer( timerID );
    
    // Set.
    if (dh != NULL)
    {
        eventHandler = dh;
        timerID = SDL_AddTimer( eventHandler->getInterval(), DispatcherCallback, 0 );
    }
}

void Dispatcher::setKeyHandler( DispatchKeyHandler* dk )
{
    keyHandler = dk;
}

void Dispatcher::mainLoop()
{
    SDL_Event Event;
    while( SDL_WaitEvent( &Event ) != 0 )
    {

        // Timer.
        if ( eventHandler != NULL && SDL_USEREVENT == Event.type )
        {
            eventHandler->tick();
            continue;
        }

        // Quit.
        if ( SDL_QUIT == Event.type )
        {
            if ( !eventHandler->quit() )
            {
                return; // Exit loop.
            }
            continue;
        }

        // Key release.
        if ( keyHandler != NULL && SDL_KEYUP == Event.type )
        {
            switch ( Event.key.keysym.sym )
            {
            case SDLK_RIGHT:
                keyHandler->releaseRight();
                continue;
            case SDLK_LEFT:
                keyHandler->releaseLeft();
                continue;
            case SDLK_DOWN:
                keyHandler->releaseDown();
                continue;
            case SDLK_UP:
                keyHandler->releaseUp();
                continue;
            default:
                continue;
            }
        }

        // Key down.
        if ( keyHandler != NULL && SDL_KEYDOWN == Event.type )
        {
            switch ( Event.key.keysym.sym )
            {
            case SDLK_ESCAPE:
                keyHandler->pressEscape();
                continue;
            case SDLK_p:
                keyHandler->pressP();
                continue;
            case SDLK_r:
                keyHandler->pressR();
                continue;
            case SDLK_PLUS:
            case SDLK_KP_PLUS:
                keyHandler->pressPlus();
                continue;
            case SDLK_MINUS:
            case SDLK_KP_MINUS:
                keyHandler->pressMinus();
                continue;
            case SDLK_RIGHT:
                keyHandler->pressRight();
                continue;
            case SDLK_DOWN:
                keyHandler->pressDown();
                continue;
            case SDLK_LEFT:
                keyHandler->pressLeft();
                continue;
            case SDLK_UP:
                keyHandler->pressUp();
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




