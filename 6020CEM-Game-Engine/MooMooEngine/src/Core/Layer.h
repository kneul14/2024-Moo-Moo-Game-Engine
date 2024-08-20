#pragma once

#include "Core/Core.h"
#include "Events/Event.h"
#include "Core/Timestep.h"

namespace MooMooEngine
{ // SubSystem manager.
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}; // start() in unity
		virtual void OnDetach() {};
		virtual void OnUpdate(Timestep ts) {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& event) {}; // Keyboard events

		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};

}