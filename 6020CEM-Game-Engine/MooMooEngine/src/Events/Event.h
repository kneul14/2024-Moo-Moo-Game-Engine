#pragma once

#include "MooMooEngine_PCH.h"
#include "Core/Core.h"

#define BIT(x) (1 << x)

namespace MooMooEngine
{
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		GamepadConnected, GamepadDisconnected, GamepadButtonPressed, GamepadButtonReleased,
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
		EventCategoryGamepad = BIT(5),
	};

	class Event
	{
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};

	class EventDispatcher
	{

	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		//F will be deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};


	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

	using EventCallbackFn = std::function<void(Event&)>;

}