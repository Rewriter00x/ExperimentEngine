#include "exppch.h"
#include "EventDispatcher.h"

namespace Exp
{
	void EventDispatcher::AddEventListener(const void* obj, EventType eventType, const EventFn& function)
	{
		m_Listeners[eventType][obj] = function;
	}

	const std::unordered_map<const void*, EventDispatcher::EventFn>* EventDispatcher::GetEventListeners(EventType Type) const
	{
		const auto it = m_Listeners.find(Type);
		return it != m_Listeners.end() ? &it->second : nullptr;
	}
}