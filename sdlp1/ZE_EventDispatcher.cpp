#include "ZE_Core.h"
#include "ZE_EventDispatcher.h"

using namespace std;

void EventDispatcher::addEventListener(SDL_EventType type, function<void(SDL_Event)> func)
{
	ZE_eventHandler.addEventFunction(type, this, func);
}

void EventDispatcher::removeEventListener(SDL_EventType type, function<void(SDL_Event)> func)
{
	ZE_eventHandler.removeEventFunction(type, this, func);
}

void EventDispatcher::removeEventListeners(SDL_EventType type)
{
	ZE_eventHandler.removeEventsOfObject(type, this);
}

void EventDispatcher::removeAllEvents()
{
	ZE_eventHandler.removeAllEventOfObject(this);
}

EventDispatcher::~EventDispatcher()
{
	removeAllEvents();
}