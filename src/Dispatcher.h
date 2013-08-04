#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "all.h"
#include "DispatchEventHandler.h"
#include "DispatchKeyHandler.h"

namespace Escarabajo
{

class Dispatcher
{
public:
    Dispatcher();
    virtual ~Dispatcher();

    /**
     * Sets/unsets the event handler (null to unset.)
     */
    void setEventHandler( DispatchEventHandler* dh );
    
    /**
     * Sets/unsets the key handler (null to unset.)
     */
    void setKeyHandler( DispatchKeyHandler* dk );

    void mainLoop();

private:

    void doTimer();

    SDL_TimerID timerID;

    DispatchEventHandler* eventHandler;
    DispatchKeyHandler* keyHandler;

};

}

#endif /* DISPATCHER_H_ */
