#pragma once
#include "MooMooEngine.h"

#include "Core/Layer.h"
#include "Events/ApplicationEvent.h"
#include "Scene/StateManager.h"

// needed for Audio:)
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")

//#define PATH ("D:\\University Y3\\6020CEM--Advanced Programming for Game Development\\6020CEM-Game-Engine\\MooMooEngine\\assets\\audio")

namespace MooMooEngine
{
	class AudioLayer : public Layer
	{
	public:
		AudioLayer();
		~AudioLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		//virtual void OnEvent(Event& event) override;
		virtual void OnUpdate(Timestep ts) override;

		EngineState currentState = EngineState::Edit;
	};
}
