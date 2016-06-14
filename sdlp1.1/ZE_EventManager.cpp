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

void EventManager::handleEvent()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			ZE_QUIT_MAIN_LOOP = true;
		}
		else
		{
			for (auto&a : AllEvents)
			{
				if (e.type == a.second.type)
				{
					a.second.func(e);
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
	AllEvents.emplace(index, temp);
	return index;
}

void EventManager::removeEventOfObject(size_t event_index)
{
	try
	{
		AllEvents.erase(AllEvents.find(event_index));
	}
	catch (const std::out_of_range&)
	{
		cout << "EventManager::removeEventOfObject invaild" << endl;
	}
}

void EventManager::removeAllEvent()
{
	AllEvents.clear();
}