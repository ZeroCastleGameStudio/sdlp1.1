#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <SDL.h>

class EventDispatcher;

//给eventDispatcher类用的元素结构体
struct EventData
{
	SDL_EventType type;
	EventDispatcher* signedObject;
	std::function<void(SDL_Event)> func;
	size_t eventIndex;
};

// 事件管理器用结构体
struct EventStruct
{
	explicit EventStruct(size_t index, size_t dispatch_index, Uint32 event_type, EventData &event_data)
		:index(index), dispatch_index(dispatch_index), event_type(event_type), event_data(event_data) {}
	size_t index;
	size_t dispatch_index;
	Uint32 event_type;
	EventData event_data;
	bool operator<(const EventStruct& o) const
	{
		return index < o.index;
	}
};

namespace EventContainerTag
{
	class Index {};
	class DispatchIndex {};
	class EventType {};
	class DispatchIndexAndEventType {};
}

// 事件管理器容器类型
using EventContainer = ::boost::multi_index_container <
	EventStruct,
	::boost::multi_index::indexed_by <
	::boost::multi_index::sequenced<
	>,
	::boost::multi_index::ordered_unique<
	::boost::multi_index::tag<EventContainerTag::Index>,
	::boost::multi_index::member<EventStruct, size_t, &EventStruct::index>
	>,
	::boost::multi_index::ordered_non_unique<
	::boost::multi_index::tag<EventContainerTag::DispatchIndex>,
	::boost::multi_index::member<EventStruct, size_t, &EventStruct::dispatch_index>
	>,
	::boost::multi_index::ordered_non_unique<
	::boost::multi_index::tag<EventContainerTag::EventType>,
	::boost::multi_index::member<EventStruct, Uint32, &EventStruct::event_type>
	>,
	::boost::multi_index::ordered_non_unique<
	::boost::multi_index::tag<EventContainerTag::DispatchIndexAndEventType>,
	::boost::multi_index::composite_key<
	EventStruct,
	::boost::multi_index::member<EventStruct, size_t, &EventStruct::dispatch_index>,
	::boost::multi_index::member<EventStruct, Uint32, &EventStruct::event_type>
	>
	>
	>
> ;

