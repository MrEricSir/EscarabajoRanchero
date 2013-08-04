#include "DispatchEventHandler.h"

using namespace Escarabajo;

DispatchEventHandler::DispatchEventHandler() { /* unused */ }

DispatchEventHandler::DispatchEventHandler( unsigned intervalMs ) : intervalMs(intervalMs) {}

DispatchEventHandler::~DispatchEventHandler()
{
    // do nothing
}


bool DispatchEventHandler::quit() { return false; }
void DispatchEventHandler::tick() {}

unsigned DispatchEventHandler::getInterval()
{
    return intervalMs;
}


