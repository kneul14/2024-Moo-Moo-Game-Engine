#pragma once

#include "Core.h"
#include "Core/KeyCodes.h"
#include "Events/Event.h"
#include <glm/glm.hpp>

namespace MooMooEngine
{
	class Input
	{

	public:

		static void Init();
		static void SetEventCallback(const EventCallbackFn& callback);

		static bool IsKeyPressed(int key);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}