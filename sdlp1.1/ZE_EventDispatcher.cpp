#include "ZE_Core.h"
#include "ZE_EventDispatcher.h"

using namespace std;

void EventDispatcher::addEventListener(SDL_EventType type, function<void(SDL_Event)> func)
{
	event_indexs.insert(make_pair(type, GlobalState->ZE_eventHandler->addEventFunction(type, this, func)));
}

void EventDispatcher::removeEventListeners(SDL_EventType type)
{
	auto itp = event_indexs.equal_range(type);
	auto it = itp.first;
	while (it != itp.second)
	{
		GlobalState->ZE_eventHandler->removeEventOfObject(it->second);
		++it;
	}

	event_indexs.erase(itp.first, itp.second);
}

void EventDispatcher::removeAllEvents()
{
	for (auto& a : event_indexs)
	{
		GlobalState->ZE_eventHandler->removeEventOfObject(a.second);
	}
	event_indexs.clear();
}

EventDispatcher::~EventDispatcher()
{
	removeAllEvents();
}