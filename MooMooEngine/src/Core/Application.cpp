#include "MooMooEngine_PCH.h"
#include "Application.h"
#include "Logging/Log.h"
#include "Physics/PhysicsLayer.h"
#include "Client/ClientLayer.h"
#include "Audio/AudioLayer.h"

#include "Input.h"

#include <GLFW/glfw3.h>

namespace MooMooEngine
{

	#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Input::Init();
		Input::SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		//Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_PhysicsLayer = new PhysicsLayer();
		PushOverlay(m_PhysicsLayer);

		m_ClientLayer = new ClientLayer();
		PushOverlay(m_ClientLayer);

		m_AudioLayer = new AudioLayer();
		PushOverlay(m_AudioLayer);
	}

	Application::~Application()
	{
		//Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled) {
				break;
			}
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime(); //Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				//Only update everything if we aren't minimized, it's pointless
				//to update a minimized window so 'pause' the game.

				//Update each layer in the layer stack. We can use this range based
				//for loop because of the 'begin' and 'end' we created in layerstack.h
				{
					for (Layer* layer : m_LayerStack) {
						layer->OnUpdate(timestep);
					}
				}

				//If not in here it allows IMGUI un-docked windows to be updated
				//even if the main window is minimized
				m_ImGuiLayer->Begin();
				{
					for (Layer* layer : m_LayerStack)
					{
						layer->OnImGuiRender();
					}
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true; //as it has been handled we can return true
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

		//Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}