#include "ZE_EventManager.h"
#include "ZE_Core.h"

using namespace std;

std::atomic_bool EventManagerSingleton_AreCtor{ false };

EventManager::EventManager()
{
	if (EventManagerSingleton_AreCtor.exchange(true))
	{
		throw std::runtime_error("");
	}
}

EventManager::~EventManager()
{
	cout << "EventManager::~EventManager()" << endl;
}

void EventManager::handleEvent()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			GlobalState->ZE_QUIT_MAIN_LOOP = true;
		}
		else
		{
			// TODO ¸ÄÐ´
			for (auto&a : AllEvents)
			{
				if (e.type == a.type)
				{
					a.event_data.func(e);
				}
			}
		}
	}
}

size_t EventManager::addEventFunction(SDL_EventType type, EventDispatcher* signedObject, function<void(SDL_Event)> func)
{
	size_t index = ++event_index;
	EventData temp;
	temp.type = type;
	temp.signedObject = signedObject;
	temp.func = func;
	temp.eventIndex = index;
	AllEvents.emplace(index, type, temp);
	return index;
}

void EventManager::removeEventOfObject(size_t event_index)
{
	AllEvents.get<EventContainerTag::Index>().erase(event_index);
}

void EventManager::removeAllEvent()
{
	AllEvents.clear();
	cout << "EventManager::removeAllEvent()" << endl;
}