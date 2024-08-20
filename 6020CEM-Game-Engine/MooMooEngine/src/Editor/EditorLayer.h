#pragma once

#include "MooMooEngine.h"
#include "PxPhysicsAPI.h"
#include "Scene/StateManager.h"
#include "Renderer/Textures/Texture2D.h"
#include <string>
#include "imgui.h"


using namespace std;

namespace MooMooEngine
{
	class EditorLayer : public Layer
	{
	public:

		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		void CreateHierarchyWindow();
		void CreateInspectorWindow();
		void CreateProfilingWindow(int& maxFPSFrames, float& memory);
		void CameraWindow();
		void LightWindow();


		void ConsoleLogging(const std::string& message) {
			m_LogBuffer.appendf("[%s] %s\n", __TIMESTAMP__, message.c_str());
		}

		enum ShapeType { Triangle, Plane, Circle, Cube, Cone, Sphere, Mesh };

		int maxFPSFrames = 2000;
		float memory = 1000.0f;

	private:
		PerspectiveCameraController m_CameraController;
		Ref<Framebuffer> m_Framebuffer;

		Ref<MeshRegistry> m_MeshRegistry;
		Ref<Scene> m_Scene;
		Entity m_monkey, m_bunny, m_cone;

	private:
		// These are smart/shared pointers of type template T.
		Ref<Shader> m_ColourShader;
		Ref<Renderer3D> m_Renderer3D;
		Ref<DynamicRigidBody> m_PhysX;
		PhysicsLayer* m_physics;
		ClientLayer* m_client;
		AudioLayer* m_audio;
		glm::vec3 m_startPosition = { 0.0f, 0.0f, 0.0f };

		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

		glm::vec4 m_defaultColour = { 1.0f, 1.0f, 1.0f, 1.0f }; 
		glm::vec4 m_colour        = { 1.0f, 1.0f, 1.0f, 1.0f };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		bool m_ViewportFocused2 = false;
		bool m_ViewportHovered2 = false;
		glm::vec2 m_ViewportSize = { 0.0f,0.0f };
		glm::vec2 m_ViewportSize2 = { 0.0f,0.0f };
		glm::vec2 m_SceneSize = { 0.0f,0.0f };


		glm::vec3 lightPos    {50.0f, 50.0f, 50.0f};		// Position of the light 
		glm::vec4 lightCol    {1.0f, 1.0f, 1.0f, 1.0f}; // White

		unsigned int texture;

		ImGuiTextBuffer m_LogBuffer;

	};

}