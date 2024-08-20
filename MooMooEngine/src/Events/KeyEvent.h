#pragma once

#include "Event.h"

namespace MooMooEngine
{

	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		int GetCategoryFlags() const override { return EventCategoryApplication | EventCategoryInput; }
	protected:
		KeyEvent(int keycode) : m_KeyCode(keycode) {}
		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{

	public:
		KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeated)";
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyPressed; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyPressed"; }

	private:
		int m_RepeatCount;
	};

	class KeyTypedEvent : public KeyEvent {

	public:
		KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyTyped; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyTyped"; }
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyReleased; }
		EventType GetEventType() const override { return GetStaticType(); }
		const char* GetName() const override { return "KeyReleased"; }
	};

}