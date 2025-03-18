#pragma once

namespace Exp
{
	template<typename T, typename F>
	static std::function<void(const Event&)> WrapEventFunction(F&& func)
	{
		return [func](const Event& e)
			{
				func(static_cast<const T&>(e));
			};
	}

	#define ADD_EVENT_LISTENER(obj, type, fn) ::Exp::Application::Get().AddEventListener(obj, EventType::type, WrapEventFunction<type##Event>([this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }))

	class EventDispatcher final
	{
	public:
		using EventFn = std::function<void(const Event&)>;

	public:
		EventDispatcher() = default;
		~EventDispatcher() = default;

		void AddEventListener(void* obj, EventType eventType, const EventFn& function);

		const std::unordered_map<const void*, EventFn>* GetEventListeners(EventType Type) const;

	private:
		std::unordered_map<EventType, std::unordered_map<const void*, EventFn>> m_Listeners;

	};
}
