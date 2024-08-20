#include "MooMooEngine_PCH.h"
#include "PerspectiveCameraController.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace MooMooEngine
{

	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspect, bool rotation)
		:m_AspectRatio(aspect), m_fov(fov),
		m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		m_Camera(fov, aspect, 1000, 0.1),
		m_Rotation(rotation)
	{

	}

	void PerspectiveCameraController::SetYView()
	{
		pitch = -90.0f;
		yaw = -270.0f;
		m_CameraPosition = glm::vec3(0.0f, 2.0f, 0.0f);  // Positionxzs the camera above the word center
		m_CameraForward = glm::vec3(0.0f, -1.0f, 0.0f);   // Looks directly down
		m_CameraUp = glm::vec3(0.0f, 0.0f, -1.0f);        
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetForward(m_CameraForward);
	}

	void PerspectiveCameraController::SetXView()
	{
		pitch = 0.0f;
		yaw = -90.0f;
		m_CameraPosition = glm::vec3(0.0f, 0.0f, 2.0f);  // Positionxzs the camera above the word center
		m_CameraForward = glm::vec3(0.0f, 0.0f, -1.0f);   // Looks directly forward
		m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetForward(m_CameraForward);
	}

	void PerspectiveCameraController::SetZView()
	{
		pitch = 0.0f;
		yaw = -180.0f;
		m_CameraPosition = glm::vec3(2.0f, 0.0f, 0.0f);  // Positionxzs the camera above the word center
		m_CameraForward = glm::vec3(-1.0f, 0.0f, 0.0f);   // Looks directly side
		m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetForward(m_CameraForward);
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(KEY_A))
		{
			m_CameraPosition -= glm::normalize(glm::cross(m_CameraForward, m_CameraUp)) * (m_CameraTranslationSpeed * ts);
		}
		else if (Input::IsKeyPressed(KEY_D))
		{
			m_CameraPosition += glm::normalize(glm::cross(m_CameraForward, m_CameraUp)) * (m_CameraTranslationSpeed * ts);
		}

		if (Input::IsKeyPressed(KEY_S))
		{
			m_CameraPosition -= m_CameraForward * (m_CameraTranslationSpeed * ts);
		}
		else if (Input::IsKeyPressed(KEY_W))
		{
			m_CameraPosition += m_CameraForward * (m_CameraTranslationSpeed * ts);
		}

		if (Input::IsKeyPressed(KEY_Q))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(KEY_E))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}

		m_Camera.SetPosition(m_CameraPosition);

		//Linear scaling of moving speed with zoom level, slower movement when zoomed in
		m_CameraTranslationSpeed = m_ZoomLevel;

	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));

		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseButtonReleased));
	}

	
	void PerspectiveCameraController::SetYaw(float y)
	{
		yaw = y;
		m_CameraRotation.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		m_CameraRotation.y = sin(glm::radians(pitch));
		m_CameraRotation.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		m_CameraForward = glm::normalize(m_CameraRotation);

		m_Camera.SetForward(m_CameraForward);
	}

	void PerspectiveCameraController::SetPitch(float p)
	{
		pitch = p;
		m_CameraRotation.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		m_CameraRotation.y = sin(glm::radians(pitch));
		m_CameraRotation.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		m_CameraForward = glm::normalize(m_CameraRotation);

		m_Camera.SetForward(m_CameraForward);
	}

	void PerspectiveCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_fov, m_AspectRatio, 1000.0f, 0.1f);
	}

	void PerspectiveCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;

		//Clamp the zoom level
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (mousePanning)
		{
			float xoffset = e.GetX() - prev_mouseX;
			float yoffset = prev_mouseY - e.GetY(); // reversed since y-coordinates range from bottom to top

			prev_mouseX = e.GetX();
			prev_mouseY = e.GetY();

			const float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			m_CameraRotation.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			m_CameraRotation.y = sin(glm::radians(pitch));
			m_CameraRotation.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			m_CameraForward = glm::normalize(m_CameraRotation);

			m_Camera.SetForward(m_CameraForward);
		}

		return false;
	}

	bool PerspectiveCameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == 1)
		{
			mousePanning = true;
		}
		return false;
	}

	bool PerspectiveCameraController::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetMouseButton() == 1)
		{
			mousePanning = false;
		}
		return false;
	}
}