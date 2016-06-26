#include "ZE_Core.h"
#include "ZE_EventDispatcher.h"
#include "ZE_EventManager.h"
#include "ZE_EventContainer.h"

using namespace std;

void EventDispatcher::addEventListener(EventMode event_mode, Uint32 type, function<void(SDL_Event)> func)
{
	if (!core_engine.expired())
		core_engine.lock()->ZE_eventHandler->addEventFunction(dispatch_index, event_mode, type, func);
}

void EventDispatcher::removeEventListeners(SDL_EventType type) const
{
	if (!core_engine.expired())
		core_engine.lock()->ZE_eventHandler->removeAllEventOfDispatchAndType(dispatch_index, type);
}

void EventDispatcher::removeEventListeners(EventMode type) const
{
	if (!core_engine.expired())
		core_engine.lock()->ZE_eventHandler->removeAllEventOfDispatchAndModeType(dispatch_index, type);
}

void EventDispatcher::removeAllEvents() const
{
	if (!core_engine.expired())
		core_engine.lock()->ZE_eventHandler->removeAllEventOfDispatch(this->dispatch_index);
}

// 所有的EventDispatcher都应该在GlobalState->ZE_eventHandler初始化后初始化
EventDispatcher::EventDispatcher(weak_ptr<ZeroEngine> core_engine)
	:enable_shared_from_this<EventDispatcher>(),
	core_engine(core_engine),
	dispatch_index(core_engine.lock()->ZE_eventHandler->dispatchIndexDistributor())
{}

EventDispatcher::EventDispatcher(weak_ptr<ZeroEngine> core_engine, size_t dispatch_index)
	: enable_shared_from_this<EventDispatcher>(),
	core_engine(core_engine),
	dispatch_index{ dispatch_index }
{
}

EventDispatcher::~EventDispatcher()
{
	removeAllEvents();
}
