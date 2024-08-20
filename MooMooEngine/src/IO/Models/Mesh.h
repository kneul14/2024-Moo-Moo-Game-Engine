#pragma once

#include "Vertex.h"
#include "Renderer/Textures/Texture.h"

namespace MooMooEngine {
	class Mesh {
	public:

		Mesh() {};
		Mesh(std::vector<Vertex> verticies, std::vector<unsigned int> indicies, std::vector<Texture*> textures);
		~Mesh() = default;

		void CreateBuffers();
		void AssignData();

		inline uint32_t GetVAO() { return m_VertexArray; }
		inline uint32_t GetVBO() { return m_VertexBuffer; };
		inline uint32_t GetIBO() { return m_IndexBuffer; };

		std::vector<Texture*> GetTextures() { return textures; };
		inline uint32_t GetIBOSize() { return indices.size(); };

	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture*> textures;
		uint32_t m_VertexArray, m_VertexBuffer, m_IndexBuffer;

	};

}