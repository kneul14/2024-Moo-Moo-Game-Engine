#include "MooMooEngine_PCH.h"
#include "Core/Input.h"

#include<GLFW/glfw3.h>
#include "Core/Application.h"

namespace MooMooEngine
{

	static EventCallbackFn s_EventCallback;

	void Input::Init()
	{

	}

	void Input::SetEventCallback(const EventCallbackFn& callback)
	{
		s_EventCallback = callback;
	}

	bool Input::IsKeyPressed(int key)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}


	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}

}