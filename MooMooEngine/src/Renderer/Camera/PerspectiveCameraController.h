#pragma once

#include "PerspectiveCamera.h"
#include "Core/Timestep.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace MooMooEngine
{
	struct PerspectiveCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};


	class PerspectiveCameraController
	{

	public:
		//default near of 0.1 and far of 1000
		PerspectiveCameraController(float fov, float aspect, bool rotation = true);
		void SetXView();
		void SetYView();
		void SetZView();

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		float GetFOV() const { return m_fov; }
		void SetFOV(float fov) { m_fov = fov; CalculateView();}

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float zoom) { m_ZoomLevel = zoom; CalculateView(); }

		const PerspectiveCameraBounds& GetBounds() const { return m_Bounds; }
		void SetBounds(const PerspectiveCameraBounds& bounds) { m_Bounds = bounds; }
				
		float GetYaw() const { return yaw; }
		void SetYaw(float y);
		
		float GetPitch() const { return pitch; }
		void SetPitch(float p);

	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);

	private:
		float m_AspectRatio;
		float m_fov;
		float m_ZoomLevel = 1.0f;
		PerspectiveCameraBounds m_Bounds;
		PerspectiveCamera m_Camera;

		bool m_Rotation;

		float prev_mouseX = 640.0f, prev_mouseY = 360.0f;
		float pitch = 0.0f, yaw = -90.0f;
		bool mousePanning = false;

		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 2.0f };
		glm::vec3 m_CameraForward  = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_CameraUp       = { 0.0f, 1.0f, 0.0f };

		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;

	};
}