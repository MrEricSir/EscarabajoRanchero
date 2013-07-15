#include "DispatchHandler.h"

using namespace Escarabajo;

DispatchHandler::DispatchHandler() { /* unused */ }

DispatchHandler::DispatchHandler( unsigned intervalMs ) : intervalMs(intervalMs) {}

DispatchHandler::~DispatchHandler()
{
	// do nothing
}


bool DispatchHandler::quit() { return false; }
void DispatchHandler::tick() {}

unsigned DispatchHandler::getInterval()
{
	return intervalMs;
}

void DispatchHandler::pressLeft() {}
void DispatchHandler::releaseLeft() {}

void DispatchHandler::pressRight() {}
void DispatchHandler::releaseRight() {}


void DispatchHandler::pressUp() {}
void DispatchHandler::releaseUp() {}

void DispatchHandler::pressDown() {}
void DispatchHandler::releaseDown() {}

void DispatchHandler::pressEscape() {}
void DispatchHandler::pressPlus() {}
void DispatchHandler::pressMinus() {}
void DispatchHandler::pressR() {}
void DispatchHandler::pressP() {}



