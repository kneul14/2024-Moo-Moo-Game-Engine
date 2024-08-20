#include "MooMooEngine_PCH.h"

#include "EditorLayer.h"
#include "imgui.h"
#include "Core/Application.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DataManagement/FileLoader.h"

#include <windows.h>
#include <pdh.h>
#pragma comment(lib, "pdh.lib")
#include <psapi.h>  // For Getting the ProcessMemoryInfo

#include <glad/glad.h>
#include <unordered_map>


namespace MooMooEngine
{
	static bool showProfilerWindow = false;
	bool isCameraSelected = false;
	bool isLightSelected = false;

	entt::entity selectedEntity = entt::null;
	Entity* theSelectedEntity = nullptr;

	EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(60.0f, 1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach() //setup
	{
		m_ColourShader = std::make_shared<Shader>("assets/shaders/FlatColourShader.glsl");

		FramebufferSpecification framebufferSpec;
		framebufferSpec.Width = 1280;
		framebufferSpec.Height = 720;
		m_Framebuffer = std::make_shared<Framebuffer>(framebufferSpec);

		m_MeshRegistry = CreateRef<MeshRegistry>();
		m_Scene = CreateRef<Scene>();

		// Flyweight Pattern in use
		m_MeshRegistry->Load("assets/models/cone.obj");

		m_Renderer3D = CreateRef<Renderer3D>();
		m_physics    = Application::Get().GetPhysicsLayer();
		m_client     = Application::Get().GetClientLayer();
		m_audio		 = Application::Get().GetAudioLayer();

		m_CameraController.SetZoomLevel(5.0f);

		ConsoleLogging("Currently in Edit Mode.");
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		//Update
		if (m_ViewportFocused)
		{
			m_CameraController.OnUpdate(ts);
		}

		{
			//Pre-Render
			m_Framebuffer->Bind();

			m_Renderer3D->PreProcessing();

		}

		{
			m_Scene->OnUpdate(ts);
		}

		{

			m_ColourShader->Bind();
			m_ColourShader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());

			m_ColourShader->SetFloat3("u_LightPos", lightPos);
			m_ColourShader->SetFloat3("u_ViewPos", m_CameraController.GetCamera().GetPosition());
			m_ColourShader->SetFloat4("u_LightColour", lightCol);
			m_ColourShader->SetFloat4("u_ObjectColour", m_colour);

			
			m_ColourShader->SetInt("u_Textures[0]", 0);

			m_Framebuffer->Unbind();
		}

	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();
		

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		#pragma region BeginMenuBar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					// Logging the new file creation
					LOG_WARN("New file");

					m_LogBuffer.clear();

					isCameraSelected = false;
					isLightSelected = false;

					selectedEntity = entt::null;

					m_CameraController.SetXView();
					lightPos = glm::vec3( 50.0f, 50.0f, 50.0f );		// Position of the light 
					lightCol = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f); // White

					m_Scene->m_Registry.clear();

					// Makenew scene
					m_Scene = std::make_shared<Scene>();
				}

				// https://www.youtube.com/watch?v=zn7N7zHgCcs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=93
				if (ImGui::MenuItem("Save"))
				{
					std::string location = "assets/GameScene.json";
					FileLoader::JSONSave(m_Scene->m_Registry, location);
				}

				if (ImGui::MenuItem("Save as..."))
				{
					std::string saveFilePath = MooMooEngine::FileLoader::SaveFile("JSON (*.json)\0*.json\0");
					if (!saveFilePath.empty()) {

						ConsoleLogging("Saving file as: " + saveFilePath);

						FileLoader::JSONSave(m_Scene->m_Registry, saveFilePath);
					}

				}

				if (ImGui::MenuItem("Open File..."))
				{
					isCameraSelected = false;
					isLightSelected = false;

					selectedEntity = entt::null;

					m_CameraController.SetXView();
					lightPos = glm::vec3(50.0f, 50.0f, 50.0f);		// Position of the light 
					lightCol = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // White

					m_Scene->m_Registry.clear();

					// Makenew scene
					m_Scene = std::make_shared<Scene>();

					// to open the file
					std::string filePath = FileLoader::OpenFile("JSON (*.json)\0*.json\0");
					if (!filePath.empty())
					{
						ConsoleLogging("Opening " + filePath);
					}

					FileLoader::JSONLoad(filePath, m_Scene, m_MeshRegistry, m_ColourShader, m_physics);

				}

				if (ImGui::MenuItem("Exit"))
				{
					dockspaceOpen = false;
					Application::Get().Close();
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Profiler"))
				{
					showProfilerWindow = !showProfilerWindow;
					
				}
				ImGui::EndMenu();
			}
			// Render "Play" button in the middle
			ImGui::SameLine(ImGui::GetWindowWidth() / 2 - 50); // Adjust position as needed
			if (ImGui::Button("Play Mode"))
			{
				ConsoleLogging("Currently in Play mode");
				StateManager::StateManager::m_SceneState = EngineState::Play;
				ImGui::SetWindowFocus("Scene Viewport");
			}
			if (ImGui::Button("Edit Mode"))
			{
				ConsoleLogging("Currently in Edit mode");
				StateManager::m_SceneState = EngineState::Edit;
				ImGui::SetWindowFocus("Viewport");
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
		#pragma endregion

		#pragma region Hierarchy
		if(StateManager::m_SceneState == EngineState::Edit) CreateHierarchyWindow();
		#pragma endregion

		#pragma region Inspector
		if (StateManager::m_SceneState == EngineState::Edit) CreateInspectorWindow();
		#pragma endregion

		#pragma region Profiler Window		
		if (showProfilerWindow)	CreateProfilingWindow(maxFPSFrames, memory);
		#pragma endregion

		#pragma region Camera Window
		if (StateManager::m_SceneState == EngineState::Edit && isCameraSelected)	CameraWindow();
		#pragma endregion

		#pragma region Light Window
		if (StateManager::m_SceneState == EngineState::Edit && isLightSelected)	LightWindow();
		#pragma endregion

		#pragma region Console Window
		ImGui::Begin("Console");
		ImGui::TextUnformatted(m_LogBuffer.begin());
		ImGui::End();
		#pragma endregion

		//// Render the Scene viewport
		#pragma region Scene Viewport
		if (StateManager::m_SceneState == EngineState::Edit) {

		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Scene Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColourAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::End();
		ImGui::PopStyleVar();
		#pragma endregion

		//// Render the viewport
		// Has to be done in this order because ImGui is weird, and it makes the user select the screen which makes it a little confusing.
		#pragma region Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize2 = ImGui::GetContentRegionAvail();

		if (m_ViewportSize2 != *((glm::vec2*)&viewportPanelSize2))
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize2.x, (uint32_t)viewportPanelSize2.y);
			m_ViewportSize2 = { viewportPanelSize2.x, viewportPanelSize2.y };

			m_CameraController.OnResize(viewportPanelSize2.x, viewportPanelSize2.y);
		}

		uint32_t textureID2 = m_Framebuffer->GetColourAttachmentRendererID();
		ImGui::Image((void*)textureID2, ImVec2{ m_ViewportSize2.x, m_ViewportSize2.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::End();
		ImGui::PopStyleVar();
		#pragma endregion

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
	
	void EditorLayer::CreateHierarchyWindow()
	{
		ImGui::Begin("Hierarchy");

		if (ImGui::IsWindowHovered()) {
			ImGui::SetTooltip("Right-click to add Objects to the scene.");
		}

		if (ImGui::IsKeyPressed(ImGuiKey_MouseRight) && ImGui::IsWindowHovered()) {
			ImGui::SetWindowFocus("Hierarchy");
			ImGui::OpenPopup("Add Something");
		}

		if (ImGui::Button("+ New Mesh"))
		{
			std::string filePath = FileLoader::OpenFile("3D Models (*.obj;*.stl;*.fbx;*.ply)\0*.obj;*.stl;*.fbx;*.ply\0All Files (*.*)\0*.*\0");
			if (!filePath.empty())
			{
			Entity userModel;
			m_MeshRegistry->Load(filePath);
			userModel = m_Scene->CreateEntity("3D Model");

			// Gwet the file name from the file path
			size_t lastSlash = filePath.find_last_of("/\\");
			std::string objName = (lastSlash == std::string::npos) ? filePath : filePath.substr(lastSlash + 1);

			ConsoleLogging("Imported File: " + filePath);

			userModel.AddComponent<ModelComponent>(Model(filePath, m_MeshRegistry, m_ColourShader));
			userModel.GetComponent<TransformComponent>().Translate(m_position);
			userModel.GetComponent<TagComponent>().Tag = objName;
			userModel.GetComponent<TransformComponent>().Scale({ 0.5f, 0.5f, 0.5f });
			userModel.AddComponent<MaterialComponent>(m_ColourShader);
			userModel.GetComponent<MaterialComponent>().Colour(m_defaultColour);
			}
			else {
				ConsoleLogging("Couldn't find file.");
			}
		}

		if (ImGui::BeginPopup("Add Something"))
		{
			ImGui::SeparatorText("2D Object");

			if (ImGui::Selectable("Triangle"))
			{
				Entity triangle = m_Scene->CreateEntity("Triangle");
				triangle.AddComponent<ShapeComponent>(Triangle);
				triangle.AddComponent<MaterialComponent>(m_ColourShader);
				triangle.GetComponent<MaterialComponent>().Colour(m_defaultColour);
			}

			if (ImGui::Selectable("Plane"))
			{
				Entity plane = m_Scene->CreateEntity("Plane");
				plane.AddComponent<ShapeComponent>(plane);
				plane.AddComponent<MaterialComponent>(m_ColourShader);
				plane.GetComponent<MaterialComponent>().Colour(m_defaultColour);
			}

			if (ImGui::Selectable("Circle"))
			{
				Entity circle = m_Scene->CreateEntity("Circle");
				circle.AddComponent<ShapeComponent>(Circle);
				circle.AddComponent<MaterialComponent>(m_ColourShader);
				circle.GetComponent<MaterialComponent>().Colour(m_defaultColour);
			}

			ImGui::SeparatorText("3D Object");

			if (ImGui::Selectable("Cube"))
			{
				Entity cube = m_Scene->CreateEntity("Cube");
				cube.AddComponent<ShapeComponent>(Cube);
				cube.AddComponent<MaterialComponent>(m_ColourShader);
				cube.GetComponent<MaterialComponent>().Colour(m_defaultColour);
			}

			if (ImGui::Selectable("Sphere"))
			{
				Entity sphere = m_Scene->CreateEntity("Sphere");
				sphere.AddComponent<ShapeComponent>(Sphere);
				sphere.AddComponent<MaterialComponent>(m_ColourShader);
				sphere.GetComponent<MaterialComponent>().Colour(m_defaultColour);
			}

			if (ImGui::Selectable("Cone"))
			{
				m_cone = m_Scene->CreateEntity("Cone");
				m_cone.AddComponent<ModelComponent>(Model("assets/models/cone.obj", m_MeshRegistry, m_ColourShader));
				m_cone.GetComponent<TransformComponent>().Translate(m_position);
				m_cone.GetComponent<TransformComponent>().Scale({ 0.3f, 0.3f, 0.3f });
				m_cone.AddComponent<MaterialComponent>(m_ColourShader);
				m_cone.GetComponent<MaterialComponent>().Colour(m_defaultColour);
			}

			ImGui::SeparatorText("Other");

			if (ImGui::Selectable("Create Empty"))
			{
				Entity empty = m_Scene->CreateEntity("GameObject");
				empty.GetComponent<TransformComponent>().Translate(m_position);
				empty.AddComponent<MaterialComponent>(m_ColourShader);
				empty.GetComponent<MaterialComponent>().Colour(m_defaultColour);
			}

			if (ImGui::Selectable("Camera"))
			{
				//Entity empty = m_Scene->CreateEntity("GameObject");
				//empty.GetComponent<TransformComponent>().Translate(m_position);
				//empty.AddComponent<MaterialComponent>(m_ColourShader);
				//empty.GetComponent<MaterialComponent>().Colour(m_defaultColour);
			}

			if (ImGui::Selectable("Light"))
			{
				//Entity empty = m_Scene->CreateEntity("GameObject");
				//empty.GetComponent<TransformComponent>().Translate(m_position);
				//empty.AddComponent<MaterialComponent>(m_ColourShader);
				//empty.GetComponent<MaterialComponent>().Colour(m_defaultColour);
			}
			
			ImGui::EndPopup();
		}

		if (ImGui::Selectable("Camera")) {
			isCameraSelected = true;
			isLightSelected = false;
			ImGui::SetWindowFocus("Camera");
		}

		if (ImGui::Selectable("Light")) {
			isCameraSelected = false;
			isLightSelected = true;
			ImGui::SetWindowFocus("Light");
		}

		#pragma region Name handling
		auto& allEntities = m_Scene->m_Registry.view<TagComponent>();
		std::unordered_map<std::string, int> nameCountMap;

		for (auto entity : allEntities) {
			TagComponent comp = allEntities.get<TagComponent>(entity);

			if (ImGui::Selectable(comp.Tag.c_str())) {
				selectedEntity = entity;
				ImGui::SetWindowFocus("Inspector");
			}
		}
		
		#pragma endregion

		ImGui::End();
	}

	void EditorLayer::CreateInspectorWindow()
	{
		ImGui::Begin("Inspector");
		if (selectedEntity == entt::null) {
			ImGui::End();
			return;
		}

		#pragma region Visual component handling
		if (ImGui::TreeNodeEx("Entity:", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Separator();
			ImGui::NewLine();

			char objectName[16] = { 0 }; // Initialize the array with null characters
			auto nameView = m_Scene->m_Registry.view<TagComponent>();
			if (nameView.contains(selectedEntity)) {
				TagComponent& tag = nameView.get<TagComponent>(selectedEntity);

				// Copy the initial name into objectName
				strncpy(objectName, tag.Tag.c_str(), sizeof(objectName) - 1);
				objectName[sizeof(objectName) - 1] = '\0'; // Ensure null-termination

				ImGui::AlignTextToFramePadding(); // Align text to the left
				ImGui::Text("Name    ");
				ImGui::SameLine();
				if (ImGui::InputText("##Name", objectName, IM_ARRAYSIZE(objectName), ImGuiInputTextFlags_EnterReturnsTrue)) {
					tag.Tag = objectName;
				}
			}

			ImGui::TreePop();
		}

		TransformComponent& transform = m_Scene->m_Registry.get<TransformComponent>(selectedEntity);
		if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Separator();
			ImGui::NewLine();

			ImGui::AlignTextToFramePadding(); // Align text to the left
			ImGui::Text("Position");
			ImGui::SameLine();
			m_position = transform.position;
			ImGui::DragFloat3("##Position", glm::value_ptr(m_position), 0.1f);
			transform.Translate(m_position);
			
			ImGui::Text("Rotation");
			ImGui::SameLine();	
			m_rotation = transform.rotation * 57.2958f; // radians to degrees
			ImGui::DragFloat3("##Rotation", glm::value_ptr(m_rotation), 1.f);
			transform.Rotation(m_rotation / 57.2958f); // degrees to radians
			ImGui::Text("Scale   ");
			ImGui::SameLine();
			m_scale = transform.scale;
			ImGui::DragFloat3("##Scale", glm::value_ptr(m_scale), 0.1f);
			transform.Scale(m_scale);

			if (m_Scene->m_Registry.all_of<PhysicsRBDComponent>(selectedEntity))
			{
				PhysicsRBDComponent& PX = m_Scene->m_Registry.get<PhysicsRBDComponent>(selectedEntity);

				//PxTransform globalPos = PxTransform(PxVec3(m_position.x, m_position.y, m_position.z));
				//PX.rbD->rbD->setGlobalPose(globalPos);
				//PX.rbD->rbD->clearForce();
				PxTransform globalPos = PxTransform(PxVec3(m_position.x, m_position.y, m_position.z));
				PX.rbD->rbD->setGlobalPose(globalPos);
				PxRigidDynamic* dynamicBody = PX.rbD->rbD->is<PxRigidDynamic>();
				if (dynamicBody && !dynamicBody->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC))
				{
					dynamicBody->clearForce();
				}


			}
			ImGui::TreePop();
		}

		MaterialComponent& mat = m_Scene->m_Registry.get<MaterialComponent>(selectedEntity);
		auto matView = m_Scene->m_Registry.view<MaterialComponent>();
		if (matView.contains(selectedEntity)) {
			MaterialComponent& material = mat;

			if (ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Separator();
				ImGui::NewLine();

				ImGui::AlignTextToFramePadding(); // Align text to the left
				ImGui::Text("Colour  ");
				ImGui::SameLine();
				m_colour = material.colour;
				ImGui::ColorEdit4("##Colour", glm::value_ptr(m_colour), 0.1f);
				material.Colour(m_colour);

				ImGui::NewLine();

				ImGuiStyle& style = ImGui::GetStyle();
				std::string label = " + New Material";
				float alignment = 0.5;

				float size = ImGui::CalcTextSize(label.c_str()).x + style.FramePadding.x * 2.0f;
				float avail = ImGui::GetContentRegionAvail().x;

				float off = (avail - size) * alignment;
				if (off > 0.0f)
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

				if (ImGui::Button(label.c_str())) {
					material.hasTexture = true;

					std::string filePath = FileLoader::OpenFile("Image Files (*.png;*.jpg;*.jpeg;*.bmp)\0*.png;*.jpg;*.jpeg;*.bmp\0");
					if (!filePath.empty())
					{
						ConsoleLogging("File Path: " + filePath);
					}
					Texture2D texture2D;
					material.textPath = filePath;
					material.texID = texture2D.LoadTexture(filePath.c_str());
				}

				ImGui::TreePop();
			}
		}

		/*auto entitiesWPX = m_Scene->m_Registry.view<PhysicsRBDComponent>();*/
		if (m_Scene->m_Registry.all_of<PhysicsRBDComponent>(selectedEntity))
		{
			PhysicsRBDComponent& PX = m_Scene->m_Registry.get<PhysicsRBDComponent>(selectedEntity);
			auto PXView = m_Scene->m_Registry.view<PhysicsRBDComponent>();
			if (PXView.contains(selectedEntity)) {
				PhysicsRBDComponent& physX = PX;

				if (ImGui::TreeNodeEx("RigidBody", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Separator();
					ImGui::NewLine();

					ImGui::Checkbox("Gravity", &PX.gravityEnabled);
					ImGui::Checkbox("Is Kinematic", &PX.isKinematic);

					ImGui::Text("Mass            ");
					ImGui::SameLine();
					float m = PX.rbD->rbD->getMass();
					ImGui::SliderFloat("##Mass ", &m, 0.0f, 100.0f, " % .3f");
					PX.rbD->rbD->setMass(m);

					ImGui::Text("Static Friction ");
					ImGui::SameLine();
					float sf = PX.rbD->baseMaterial->getStaticFriction();
					ImGui::SliderFloat("##Static Friction ", &sf, 0.0f, 1.0f, " % .3f");
					PX.rbD->baseMaterial->setStaticFriction(sf);

					ImGui::Text("Dynamic Friction");
					ImGui::SameLine();
					float df = PX.rbD->baseMaterial->getDynamicFriction();
					ImGui::SliderFloat("##Dynamic Friction", &df, 0.0f, 1.0f, " % .3f");
					PX.rbD->baseMaterial->setDynamicFriction(df);

					ImGui::Text("Restitution     ");
					ImGui::SameLine();
					float r = PX.rbD->baseMaterial->getRestitution();
					ImGui::SliderFloat("##Restitution     ", &r, 0.0f, 1.0f, " % .3f");
					PX.rbD->baseMaterial->setRestitution(r);

					ImGui::TreePop();
				}
			}
		}

		if (m_Scene->m_Registry.all_of<BoxColliderComponent>(selectedEntity))
		{
			BoxColliderComponent& PX = m_Scene->m_Registry.get<BoxColliderComponent>(selectedEntity);
			auto boxView = m_Scene->m_Registry.view<BoxColliderComponent>();
			if (boxView.contains(selectedEntity)) {
				BoxColliderComponent& physX = PX;

				if (ImGui::TreeNodeEx("Box Collider", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Separator();
					ImGui::NewLine();

					ImGui::Checkbox("Is Trigger", &PX.isTrigger);
				
					ImGui::TreePop();
				}
			}
		}

		if (m_Scene->m_Registry.all_of<SphereColliderComponent>(selectedEntity))
		{
			SphereColliderComponent& PX = m_Scene->m_Registry.get<SphereColliderComponent>(selectedEntity);
			auto sphereView = m_Scene->m_Registry.view<SphereColliderComponent>();
			if (sphereView.contains(selectedEntity))
			{
				SphereColliderComponent& physX = PX;

				if (ImGui::TreeNodeEx("Sphere Collider", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Separator();
					ImGui::NewLine();

					ImGui::Checkbox("Is Trigger", &PX.isTrigger);

					ImGui::Text("Radius");
					ImGui::DragFloat("##Radius", &PX.radius, 0.1f);

					ImGui::Text("Position");
					ImGui::DragFloat3("##Position", glm::value_ptr(PX.position), 0.1f);


					ImGui::TreePop();
				}
			}
		}

		#pragma region delete GO
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
		// Button deleting an entity
		ImGuiStyle& style = ImGui::GetStyle();
		std::string label = "Delete GameObject";
		float alignment = 0.5;

		float size = ImGui::CalcTextSize(label.c_str()).x + style.FramePadding.x * 2.0f;
		float avail = ImGui::GetContentRegionAvail().x;

		float off = (avail - size) * alignment;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

		if (ImGui::Button(label.c_str())) {
			LOG_WARN("GameObject was deleted");

			auto view = m_Scene->m_Registry.view < TransformComponent >();
			TransformComponent& transform = view.get<TransformComponent>(selectedEntity);

			if (m_Scene->m_Registry.all_of<DynamicRigidBody>(selectedEntity)) {
				DynamicRigidBody& rbComponent = m_Scene->m_Registry.get<DynamicRigidBody>(selectedEntity);
				rbComponent.~DynamicRigidBody(); // Call destructor explicitly
				m_Scene->m_Registry.remove<DynamicRigidBody>(selectedEntity);
			}

			// TODO:: destroy colliders that may exist

			m_Scene->m_Registry.destroy(selectedEntity);
			selectedEntity = entt::null;

		}
			#pragma endregion

		#pragma endregion

		// Handling mouse right-click and window hover 
		// If the inspector tab is right clicked the the Components tab will open
		if (ImGui::IsWindowHovered()) {
			ImGui::SetTooltip("Right-click to add components after selecting an object in the Hierarchy window.");
		}

		if (ImGui::IsKeyPressed(ImGuiKey_MouseRight) && ImGui::IsWindowHovered()) 
		{
			ImGui::SetWindowFocus("Inspector");
			ImGui::OpenPopup("Add Item");
		}

		#pragma region Component addition
		// Popup for adding items
		if (ImGui::BeginPopup("Add Item")) 
		{
			ImGui::SeparatorText("Add Component to object##1");

			if (ImGui::Selectable("Material")) 
			{
				// selected object bool set to true.
				auto allEntities = m_Scene->m_Registry.view < MaterialComponent >();
				// Changes the actual material with &
				MaterialComponent& mat = allEntities.get < MaterialComponent >(selectedEntity);
			}

			if (ImGui::BeginMenu("Physics")) 
			{
				// TODO:: check if already had RB like how we do with collider

				if (ImGui::Selectable("Dynamic RigidBody")) 
				{
					DynamicRigidBody* RBD = new DynamicRigidBody(m_physics->physics, m_physics->scene, m_startPosition);
					PhysicsRBDComponent PX = PhysicsRBDComponent(RBD);
					PX.hasPXComp = true;
					m_Scene->m_Registry.emplace<PhysicsRBDComponent>(selectedEntity, PX);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Collider")) {
				// Try to get the BoxColliderComponent for the selected entity
				BoxColliderComponent* coll = m_Scene->m_Registry.try_get<BoxColliderComponent>(selectedEntity);
				if (!coll) {
					if (ImGui::Selectable("Box Collider")) {
						// Check if the entity has a PhysicsRBDComponent
						PhysicsRBDComponent* rigidBody = m_Scene->m_Registry.try_get<PhysicsRBDComponent>(selectedEntity);
						if (rigidBody) {
							// Add a new BoxColliderComponent if the entity has a PhysicsRBDComponent
							TransformComponent& transform = m_Scene->m_Registry.get<TransformComponent>(selectedEntity);

							PhysXBoxCollider* boxColl = new PhysXBoxCollider(m_physics->physics, m_physics->scene, rigidBody->rbD->rbD, rigidBody->rbD->baseMaterial, (transform.scale / 2.0f));
							BoxColliderComponent boxCollComp = BoxColliderComponent(boxColl);
							boxCollComp.hasBoxComp = true;
							m_Scene->m_Registry.emplace<BoxColliderComponent>(selectedEntity, boxCollComp);
						}
						else {
							// If no PhysicsRBDComponent, create one and then add the BoxColliderComponent
							DynamicRigidBody* rigidBody = new DynamicRigidBody(m_physics->physics, m_physics->scene, m_startPosition);
							PhysicsRBDComponent PX = PhysicsRBDComponent(rigidBody);
							PX.hasPXComp = true;
							m_Scene->m_Registry.emplace<PhysicsRBDComponent>(selectedEntity, PX);

							PhysicsRBDComponent* rigidBodyCheck = m_Scene->m_Registry.try_get<PhysicsRBDComponent>(selectedEntity);
							TransformComponent& transform = m_Scene->m_Registry.get<TransformComponent>(selectedEntity);

							PhysXBoxCollider* boxCollider = new PhysXBoxCollider(m_physics->physics, m_physics->scene, rigidBodyCheck->rbD->rbD, rigidBodyCheck->rbD->baseMaterial, (transform.scale / 2.0f));
							BoxColliderComponent boxCollComp = BoxColliderComponent(boxCollider);
							boxCollComp.hasBoxComp = true;
							m_Scene->m_Registry.emplace<BoxColliderComponent>(selectedEntity, boxCollComp);
						}
					}
				}

				// Check if the entity has a SphereColliderComponent
				SphereColliderComponent* sphereColl = m_Scene->m_Registry.try_get<SphereColliderComponent>(selectedEntity);
				if (!sphereColl) {
					if (ImGui::Selectable("Sphere Collider")) {
						// Check if the entity has a PhysicsRBDComponent
						PhysicsRBDComponent* rigidBody = m_Scene->m_Registry.try_get<PhysicsRBDComponent>(selectedEntity);
						if (rigidBody) {
							// Add a new SphereColliderComponent if the entity has a PhysicsRBDComponent
							TransformComponent& transform = m_Scene->m_Registry.get<TransformComponent>(selectedEntity);

							// Calculate the radius of the sphere collider (assuming scale is uniform)
							float sphereRadius = glm::compMax(transform.scale) / 2.0f;

							PhysXSphereCollider* sphereColl = new PhysXSphereCollider(m_physics->physics, m_physics->scene, rigidBody->rbD->rbD, rigidBody->rbD->baseMaterial, sphereRadius);
							SphereColliderComponent sphereCollComp = SphereColliderComponent(sphereColl);
							//sphereCollComp.hasSphereComp = true;
							m_Scene->m_Registry.emplace<SphereColliderComponent>(selectedEntity, sphereCollComp);
						}
						else {
							// If no PhysicsRBDComponent, create one and then add the SphereColliderComponent
							DynamicRigidBody* rigidBody = new DynamicRigidBody(m_physics->physics, m_physics->scene, m_startPosition);
							PhysicsRBDComponent PX = PhysicsRBDComponent(rigidBody);
							PX.hasPXComp = true;
							m_Scene->m_Registry.emplace<PhysicsRBDComponent>(selectedEntity, PX);

							PhysicsRBDComponent* rigidBodyCheck = m_Scene->m_Registry.try_get<PhysicsRBDComponent>(selectedEntity);
							TransformComponent& transform = m_Scene->m_Registry.get<TransformComponent>(selectedEntity);

							// Calculate the radius of the sphere collider
							float sphereRadius = glm::compMax(transform.scale) / 2.0f;

							PhysXSphereCollider* sphereCollider = new PhysXSphereCollider(m_physics->physics, m_physics->scene, rigidBodyCheck->rbD->rbD, rigidBodyCheck->rbD->baseMaterial, sphereRadius);
							SphereColliderComponent sphereCollComp = SphereColliderComponent(sphereCollider);
							//sphereCollComp.hasSphereComp = true;
							m_Scene->m_Registry.emplace<SphereColliderComponent>(selectedEntity, sphereCollComp);
						}
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}
		#pragma endregion 

		ImGui::End();
	}

	void EditorLayer::CameraWindow() {
		ImGui::Begin("Camera");
		#pragma region Camera Handling
		if (isCameraSelected) {
			if (ImGui::TreeNodeEx("Camera Controls", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Separator();
				ImGui::NewLine();

				static bool setToDefault = true;
				ImGui::Checkbox("Set to Default", &setToDefault);
				ImGui::NewLine();

				ImGui::SeparatorText("Transform controls");

				// Camera All variables being set.
				ImGui::Text("Position     ");
				ImGui::SameLine();
				glm::vec3 cameraPosition = m_CameraController.GetCamera().GetPosition();
				ImGui::DragFloat3("##CamPosition", glm::value_ptr(cameraPosition), 0.1f);
				m_CameraController.GetCamera().SetPosition(cameraPosition);

				ImGui::Text("Yaw          ");
				ImGui::SameLine();
				float yaw = m_CameraController.GetYaw();
				ImGui::SliderFloat("##Yaw", &yaw, -270.0f, 90.0f);
				m_CameraController.SetYaw(yaw);

				ImGui::Text("Pitch        ");
				ImGui::SameLine();
				float picth = m_CameraController.GetPitch();
				ImGui::SliderFloat("##Pitch", &picth, -90.0f, 90.0f);
				m_CameraController.SetPitch(picth);

				ImGui::Text("Field of View");
				ImGui::SameLine();
				float fov = m_CameraController.GetFOV();
				ImGui::SliderFloat("##CamFOV", &fov, 1.0f, 120.0f);
				m_CameraController.SetFOV(fov);


				ImGui::NewLine();
				ImGui::SeparatorText("Axis controls");
				
				// Button for X-View
				if (ImGui::Button("X-View")) {
					LOG_INFO("X-View button was clicked");
					m_CameraController.SetXView();
				}	
				ImGui::SameLine();
				if (ImGui::Button("Y-View")) {
					LOG_INFO("Y-View button was clicked");
					m_CameraController.SetYView();
				}
				ImGui::SameLine();
				if (ImGui::Button("Z-View")) {
					LOG_INFO("Z-View button was clicked");
					m_CameraController.SetZView();
				}

				ImGui::TreePop();
			}
		}
		#pragma endregion
		ImGui::End();
	}

	void EditorLayer::LightWindow() {
		ImGui::Begin("Light");

		if (isLightSelected) {
			if (ImGui::TreeNodeEx("Lighting Controls", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Separator();
				ImGui::NewLine();

				ImGui::SeparatorText("Transformation Controls");

				ImGui::Text("Position");
				ImGui::SameLine();
				ImGui::DragFloat3("##LightPosition", glm::value_ptr(lightPos), 0.1f);

				ImGui::SeparatorText("Colour Settings");

				ImGui::Text("Colour");
				ImGui::SameLine();
				ImGui::ColorEdit4("##LightColour", glm::value_ptr(lightCol), 0.1f);

				ImGui::TreePop();
			}
		}

		ImGui::End();
	}

	float GetCPUUsage()
	{
		PDH_HQUERY query;             // Declare a query handle
		PDH_HCOUNTER counter;         // Declare a counter handle
		PDH_FMT_COUNTERVALUE value;   // Variable to store the counter value

		PdhOpenQuery(NULL, 0, &query);
		PdhAddEnglishCounter(query, L"\\Processor(_Total)\\% Processor Time", 0, &counter);
		PdhCollectQueryData(query);
		Sleep(1000);      // Waits for 1 second to collect  data
		PdhCollectQueryData(query);
		PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value);
		PdhCloseQuery(query);

		return static_cast<float>(value.doubleValue);
	}

	float GetMemoryUsage()
	{
		PROCESS_MEMORY_COUNTERS_EX pmc;   // Structure to store memory info
		pmc.cb = sizeof(pmc);			  // Set the size of the structure

		if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
			return static_cast<float>(pmc.WorkingSetSize) / (1024 * 1024);  // Memory usage in MB
		}

		return 0.0f;  // Return 0 if unable to retrieve memory info
	}

	void MooMooEngine::EditorLayer::CreateProfilingWindow(int& maxFPSFrames, float& memory)
	{
		static float fpsHistory[1200] = { 0 };  // History of FPS values, assuming 120 frames
		static int fpsHistoryIndex = 0;			// Current index in the history buffer

		static float cpuHistory[120] = { 0 };
		static float memoryHistory[100] = { 0 };
		static int historyIndex = 0;

		// this records the FPS hisrtoy
		fpsHistory[fpsHistoryIndex] = ImGui::GetIO().Framerate;
		fpsHistoryIndex = (fpsHistoryIndex + 1) % 1200;  // Circular buffer

		ImGui::Begin("Profiler");
		ImGui::Text("Profiling Information:");
		ImGui::NewLine();
		static bool showFPS = false;
		static bool showCPU = false;
		static bool showMemory = false;
		ImGui::Checkbox("FPS", &showFPS);
		ImGui::Checkbox("CPU", &showCPU);
		ImGui::Checkbox("Memory", &showMemory);

		if (showFPS) {
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::SeparatorText("Frame Rate:");

			ImGui::PlotLines(" ", fpsHistory, IM_ARRAYSIZE(fpsHistory), fpsHistoryIndex, ("Max: " + std::to_string(maxFPSFrames) + " frames").c_str(), 0.0f, static_cast<float>(maxFPSFrames), ImVec2(0, 80));
			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

			// Slider to change max frames
			ImGui::SliderInt("Max Frames", &maxFPSFrames, 10, 2000, "Max: %d frames");

		}

		if (showCPU) {
			float cpuUsage = GetCPUUsage();

			if (historyIndex < IM_ARRAYSIZE(cpuHistory)) {
				cpuHistory[historyIndex] = cpuUsage;
			}
			else {
				memmove(cpuHistory, cpuHistory + 1, (IM_ARRAYSIZE(cpuHistory) - 1) * sizeof(float));
				cpuHistory[IM_ARRAYSIZE(cpuHistory) - 1] = cpuUsage;
			}

			ImGui::NewLine();
			ImGui::SeparatorText("CPU Usage:");
			ImGui::PlotLines("CPU%", cpuHistory, IM_ARRAYSIZE(cpuHistory), historyIndex, "", 0.0f, 10.0f, ImVec2(0, 80));
			ImGui::Text("Current CPU Usage: %.1f%%", cpuUsage);
		}

		if (showMemory) {
			float memoryUsage = GetMemoryUsage();

			// Store memory usage in history buffer
			if (historyIndex < IM_ARRAYSIZE(memoryHistory)) {
				memoryHistory[historyIndex] = memoryUsage;
			}
			else {
				memmove(memoryHistory, memoryHistory + 1, (IM_ARRAYSIZE(memoryHistory) - 1) * sizeof(float));
				memoryHistory[IM_ARRAYSIZE(memoryHistory) - 1] = memoryUsage;
			}

			ImGui::NewLine();
			ImGui::SeparatorText("Memory Usage:");
			ImGui::PlotLines("Memory (MB)", memoryHistory, IM_ARRAYSIZE(memoryHistory), historyIndex, ("Max: " + std::to_string(memory)).c_str(), 0.0f, memory, ImVec2(0, 80));
			ImGui::Text("Current Memory Usage: %.2f MB", memoryUsage);
			// Slider to change max memory
			ImGui::SliderFloat("Max ", &memory, 0.0f, 10000.0f, "Max: %.2f memory");
		}

		historyIndex = (historyIndex + 1) % 1200; // Circular buffer index

		ImGui::End();
	}
}