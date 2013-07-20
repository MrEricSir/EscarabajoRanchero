#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "all.h"
#include "DispatchHandler.h"

namespace Escarabajo
{

class Dispatcher
{
public:
    Dispatcher();
    virtual ~Dispatcher();

    void setDispatchHandler( DispatchHandler* dh );
    void unsetHandler();

    void mainLoop();

private:

    void doTimer();

    SDL_TimerID timerID;

    DispatchHandler* myHandler;

};

}

#endif /* DISPATCHER_H_ */
