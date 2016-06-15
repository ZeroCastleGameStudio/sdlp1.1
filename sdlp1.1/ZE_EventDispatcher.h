#pragma once
#include <functional>
#include <SDL.h>
#include <atomic>
#include <unordered_map>
#include "ZE_EventContainer.h"

using namespace std;

//所有可注册事件类的基类
class EventDispatcher
{
public:
	EventDispatcher();
	virtual ~EventDispatcher();
	void addEventListener(EventMode event_mode, Uint32 type, function<void(SDL_Event)>);
	void removeEventListeners(SDL_EventType type) const;
	void removeAllEvents() const;
private:
	atomic_size_t dispatch_index{ ~0 };
};

