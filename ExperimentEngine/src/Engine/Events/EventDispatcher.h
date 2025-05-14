#pragma once

namespace Exp
{
	template<typename T, typename F>
#ifdef EXP_WINDOWS
    constexpr
#endif
    inline static std::function<bool(const Event&)> WrapEventFunction(F&& func)
	{
		return [func](const Event& e) -> decltype(auto)
			{
				return func(static_cast<const T&>(e));
			};
	}

	#define ADD_EVENT_LISTENER(obj, type, fn) ::Exp::Application::Get().AddEventListener(obj, EventType::type, WrapEventFunction<type##Event>([obj](auto&&... args) -> decltype(auto) { return obj->fn(std::forward<decltype(args)>(args)...); }))
	#define REMOVE_EVENT_LISTENER(obj, type) ::Exp::Application::Get().RemoveEventListener(obj, EventType::type)

	class EventDispatcher final
	{
	public:
		using EventFn = std::function<bool(const Event&)>;

	public:
		EventDispatcher() = default;
		~EventDispatcher() = default;

		void AddEventListener(const void* obj, EventType eventType, const EventFn& function);
		void RemoveEventListener(const void* obj, EventType eventType);

		const std::unordered_map<const void*, EventFn>* GetEventListeners(EventType Type) const;

	private:
		std::unordered_map<EventType, std::unordered_map<const void*, EventFn>> m_Listeners;

	};
}
