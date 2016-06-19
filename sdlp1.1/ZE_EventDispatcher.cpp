#include "ZE_Core.h"
#include "ZE_EventDispatcher.h"
#include "ZE_Global.h"
#include "ZE_EngineGlobalState.h"
#include "ZE_EventManager.h"

using namespace std;

void EventDispatcher::addEventListener(EventMode event_mode, Uint32 type, function<void(SDL_Event)> func)
{
	GlobalState->ZE_eventHandler->addEventFunction(dispatch_index, event_mode, type, func);
}

void EventDispatcher::removeEventListeners(SDL_EventType type) const
{
	GlobalState->ZE_eventHandler->removeAllEventOfDispatchAndType(dispatch_index, type);
}

void EventDispatcher::removeEventListeners(EventMode type) const
{
	GlobalState->ZE_eventHandler->removeAllEventOfDispatchAndModeType(dispatch_index, type);
}

void EventDispatcher::removeAllEvents() const
{
	GlobalState->ZE_eventHandler->removeAllEventOfDispatch(this->dispatch_index);
}

// 所有的EventDispatcher都应该在GlobalState->ZE_eventHandler初始化后初始化
EventDispatcher::EventDispatcher()
	:dispatch_index(GlobalState->ZE_eventHandler->dispatchIndexDistributor())
{}

EventDispatcher::EventDispatcher(size_t dispatch_index)
	: dispatch_index{ dispatch_index }
{
}

EventDispatcher::~EventDispatcher()
{
	removeAllEvents();
}
