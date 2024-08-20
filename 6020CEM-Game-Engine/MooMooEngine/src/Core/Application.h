#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Core/Timestep.h"
#include "ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace MooMooEngine
{
	class PhysicsLayer;
	class ClientLayer;
	class AudioLayer;

	class Application
	{
	public:
		Application(const std::string& name = "MooMooEngine");
		~Application();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		PhysicsLayer* GetPhysicsLayer() { return m_PhysicsLayer; }

		ClientLayer* GetClientLayer() { return m_ClientLayer; }
		
		AudioLayer* GetAudioLayer() { return m_AudioLayer; }

		static Application& Get() { return *s_Instance; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		PhysicsLayer* m_PhysicsLayer;
		ClientLayer* m_ClientLayer;
		AudioLayer* m_AudioLayer;

		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);

	};

	//to be defined in client
	Application* CreateApplication();

};