#include "MooMooEngine_PCH.h"
#include "WindowsWindow.h"

#include <Events/ApplicationEvent.h>
#include <Events/KeyEvent.h>
#include <Events/MouseEvent.h>

#include "GraphicsAPI/OpenGL/OpenGLContext.h"

namespace MooMooEngine
{

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int errorCode, const char* errorMessage)
	{
		LOG_ERROR("GLFW Error: ({0}) - {1}", errorCode, errorMessage);
	}

	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
		return std::make_unique<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		LOG_INFO("Create Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			//TODO: glfwTerminate on system shutdown
			int success = glfwInit();

			//set error callback
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();


		//Each window has a user pointer that can be set with glfwSetWindowUserPointer and queried with glfwGetWindowUserPointer.
		//This can be used for any purpose you need and will not be modified by GLFW throughout the life-time of the window.
		//This is an easy way of us getting a reference to our m_Data which stores all our window data such as size and event callback.
		glfwSetWindowUserPointer(m_Window, &m_Data);

		SetVSync(true);

		//Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				//Update the values stored in data
				data.Width = width;
				data.Height = height;

				WindowResizeEvent ev(width, height);

				//dispatch the event
				data.EventCallback(ev);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent ev;
				data.EventCallback(ev);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent ev(key, 0);
				data.EventCallback(ev);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent ev(key);
				data.EventCallback(ev);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent ev(key, 1);
				data.EventCallback(ev);
				break;
			}
			}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent ev(keycode);
			data.EventCallback(ev);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent ev(button);
				data.EventCallback(ev);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent ev(button);
				data.EventCallback(ev);
				break;
			}
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent ev((float)xOffset, (float)yOffset);
			data.EventCallback(ev);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent ev((float)xPos, (float)yPos);
			data.EventCallback(ev);
			});

		GLFWimage images[1];
		images[0].pixels = stbi_load("./assets/textures/CuteCow.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 

		glfwSetWindowIcon(m_Window, 1, images);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);

		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) {
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}


}
