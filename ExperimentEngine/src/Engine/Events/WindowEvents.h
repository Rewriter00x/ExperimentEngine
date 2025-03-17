#pragma once

#include "Event.h"

namespace Exp
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		DEFINE_EVENT_TYPE(WindowClose)

	};
}