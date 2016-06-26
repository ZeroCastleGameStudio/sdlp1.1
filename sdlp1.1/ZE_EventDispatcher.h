#pragma once
#include <functional>
#include <SDL.h>
#include <memory>

using namespace std;

enum class EventMode;
class ZeroEngine;

//所有可注册事件类的基类
class EventDispatcher
	:public std::enable_shared_from_this<EventDispatcher>
{
public:
	explicit EventDispatcher(weak_ptr<ZeroEngine> core_engine);
	EventDispatcher(weak_ptr<ZeroEngine> core_engine, size_t dispatch_index);
	virtual ~EventDispatcher();
	void addEventListener(EventMode event_mode, Uint32 type, function<void(SDL_Event)>);
	void removeEventListeners(SDL_EventType type) const;
	void removeEventListeners(EventMode type) const;
	void removeAllEvents() const;
public:
	weak_ptr<ZeroEngine> core_engine;
	// 事件处理器标号
	const size_t dispatch_index;
};

