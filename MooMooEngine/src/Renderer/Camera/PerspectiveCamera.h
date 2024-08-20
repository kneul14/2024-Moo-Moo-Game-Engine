#pragma once

#include <glm/glm.hpp>

namespace MooMooEngine
{

	class PerspectiveCamera
	{

	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(float fov, float aspect, float zFar, float zNear);

		void SetProjection(float fov, float aspect, float zFar, float zNear);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3 position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::vec3& GetRotation() const { return m_Rotation; }
		void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::vec3& GetForward() const { return m_Front; }
		void SetForward(const glm::vec3 forward) { m_Front = forward; RecalculateViewMatrix(); }

		const glm::vec3& GetCameraUpDirection() const { return m_CameraUp; }
		void SetCameraUpDirection(const glm::vec3 up) { m_CameraUp = up; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }


	private:
		void RecalculateViewMatrix();

		void UpdateCameraVectors();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		//Storing Euler Angles
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };

		glm::vec3 m_TargetPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Direction = { 0.0f, 0.0f, 0.0f };

		glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// euler Angles
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;


	};


}