#include "ZE_Event.h"
#include "ZE_Core.h"

using namespace std;

void Event::handleEvent()
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
			for (unsigned int i = 0; i < AllEvents.size(); i++)
			{
				if (e.type == AllEvents[i].type)
				{
					AllEvents[i].func(e);
				}
			}
		}
	}
}

void Event::addEventFunction(SDL_EventType type, EventDispatcher* signedObject, function<void(SDL_Event)> func)
{
	EventData temp;
	temp.type = type;
	temp.signedObject = signedObject;
	temp.func = func;
	AllEvents.push_back(temp);
}

void Event::removeEventFunction(SDL_EventType type, EventDispatcher* signedObject, function<void(SDL_Event)> func)
{
	for (unsigned int i = 0; i < AllEvents.size(); i++)
	{
		/*if (type == AllEvents[i].type && signedObject == AllEvents[i].signedObject && func.target == AllEvents[i].func.target)
		{
			AllEvents.erase(AllEvents.begin() + i);
		}*/
	}
}

void Event::removeEventsOfObject(SDL_EventType type, EventDispatcher* signedObject)
{
	for (unsigned int i = 0; i < AllEvents.size(); i++)
	{
		if (type == AllEvents[i].type && signedObject == AllEvents[i].signedObject)
		{
			AllEvents.erase(AllEvents.begin() + i);
		}
	}
}

void Event::removeAllEventOfObject(EventDispatcher* signedObject)
{
	for (unsigned int i = 0; i < AllEvents.size(); i++)
	{
		if (signedObject == AllEvents[i].signedObject)
		{
			AllEvents.erase(AllEvents.begin() + i);
		}
	}
}