#pragma once
#include "MooMooEngine.h"
using namespace std;

namespace MooMooEngine
{
	enum class EngineState
	{
		Edit = 0, Play = 1
	};

	class StateManager
	{
	public:
		StateManager() = default;
		~StateManager();

		static EngineState m_SceneState;
	};
}