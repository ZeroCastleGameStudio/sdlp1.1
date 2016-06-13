#pragma once
#include <functional>
#include <SDL.h>

using namespace std;

//所有可注册事件类的基类
class EventDispatcher
{
public:
	virtual ~EventDispatcher();
	void addEventListener(SDL_EventType type, function<void(SDL_Event)>);
	void removeEventListeners(SDL_EventType type);
	void removeAllEvents();
};

//给eventDispatcher类用的元素结构体
struct EventData
{
	SDL_EventType type;
	EventDispatcher* signedObject;
	function<void(SDL_Event)> func;
};