#pragma once
#include "IO/Models/Model.h"
#include <Glad/glad.h>
#include "Renderer/Shader/Shader.h"
#include <vector>
#include "stb_image.h"

namespace MooMooEngine {

	class Renderer3D
	{
	public:
		Renderer3D();
		~Renderer3D() = default;

		void PreProcessing();

		void RenderModel(Model model, glm::mat4 transform);

		void SetupTriangle();
		void SetupSquare();
		void SetupCircle();
		void SetupCube();
		void SetupSphere();

		void drawTriangle();
		void drawSquare();
		void drawCircle();
		void drawCube();
		void drawSphere();

		unsigned int textureID;

	private:
		uint32_t m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		uint32_t m_TriangleVAO, m_TriangleVBO, m_TriangleEBO;
		uint32_t m_SquareVAO, m_SquareVBO, m_SquareEBO;
		uint32_t m_CircleVAO, m_CircleVBO, m_CircleEBO;
		uint32_t m_CubeVAO, m_CubeVBO, m_CubeEBO;
		uint32_t m_ConeVAO, m_ConeVBO, m_ConeEBO;
		uint32_t m_SphereVAO, m_SphereVBO, m_SphereEBO;
	};
}
