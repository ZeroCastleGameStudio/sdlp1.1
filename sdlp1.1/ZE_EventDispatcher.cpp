#include "ZE_Core.h"
#include "ZE_EventDispatcher.h"

using namespace std;

// FIXME：这个函数不能在多线程中调用，因为GlobalState没法在构造函数中初始化，现在在此类型的某个对象初始化时间比GlobalState早
void EventDispatcher::addEventListener(EventMode event_mode, SDL_EventType type, function<void(SDL_Event)> func)
{
	if (~0 == dispatch_index.load())
	{
		dispatch_index.store(GlobalState->ZE_eventHandler->dispatchIndexDistributor());
	}
	GlobalState->ZE_eventHandler->addEventFunction(dispatch_index, event_mode, type, this, func);
}

void EventDispatcher::removeEventListeners(SDL_EventType type) const
{
	GlobalState->ZE_eventHandler->removeAllEventOfDispatchAndType(dispatch_index, type);
}

void EventDispatcher::removeAllEvents() const
{
	GlobalState->ZE_eventHandler->removeAllEventOfDispatch(this->dispatch_index);
}

EventDispatcher::EventDispatcher()
{}

EventDispatcher::~EventDispatcher()
{
	removeAllEvents();
}
