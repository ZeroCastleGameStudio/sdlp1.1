#pragma once
#include <deque>
#include <functional>
#include <SDL2/SDL.h>
#include "ZE_EventDispatcher.h"

using namespace std;

//事件统筹类
//所有eventDispatcher对象注册监听时，都调用该类的add方法
//监听方法的指针将会被保存在统筹数组里，捕获事件后就对比然后挨个调用
class Event
{
public:
	void handleEvent();
	void addEventFunction(SDL_EventType type, EventDispatcher*, function<void(SDL_Event)>);
	void removeEventFunction(SDL_EventType type, EventDispatcher*, function<void(SDL_Event)>);
	void removeEventsOfObject(SDL_EventType type, EventDispatcher*);
	void removeAllEventOfObject(EventDispatcher*);

	/*也不知道写到哪，就写到这好了
	SDL很基础，所以ZE目前对鼠标点击事件的支持不是很好
	因为所有的事件都是由该类统一调度处理的，也就是说
	当一个鼠标事件被触发时，它只会向所有已注册的方法派发此事件
	也就是说，不管图像的表现如何，即使A元素被B元素盖着，A元素也会接收到鼠标事件
	换句话说，就是不存在“遮住鼠标”的概念
	这个东西的实现感觉会很复杂，因为以目前的状况来看，它只能在触发时由子至父遍历一遍所有舞台元素，
	才能知道谁盖住了谁。或者反向遍历events数组，然后只派发一个之类的
	总之目前先不做了*/

private:
	//保存所有的事件
	deque<EventData> AllEvents;
};