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

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32 width, uint32 height)
			: m_Width(width), m_Height(height) {}

		inline uint32 GetWidth() const { return m_Width; }
		inline uint32 GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		DEFINE_EVENT_TYPE(WindowResize)

	private:
		uint32 m_Width, m_Height;

	};
}