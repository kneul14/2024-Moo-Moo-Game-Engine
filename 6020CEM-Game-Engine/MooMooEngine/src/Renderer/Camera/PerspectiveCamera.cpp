#pragma once

#include "MooMooEngine_PCH.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace MooMooEngine
{

	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float zFar, float zNear)
		:m_ProjectionMatrix(glm::perspective(fov, aspect, zNear, zFar)), m_ViewMatrix(1.0f)
	{
		RecalculateViewMatrix();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjection(float fov, float aspect, float zFar, float zNear)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		m_Direction = glm::normalize(m_Position - m_TargetPosition);

		UpdateCameraVectors();

		// the view matrix is calculated using Euler Angles and the LookAt Matrix
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_CameraUp);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}

	void PerspectiveCamera::UpdateCameraVectors()
	{
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_CameraUp = glm::normalize(glm::cross(m_Right, m_Front));

	}

}