#pragma once
#include "MeshRegistry.h"
#include "Renderer/Shader/Shader.h"
#include "Core/Core.h"
#include "Mesh.h"

namespace MooMooEngine {
	class Model {
	public:
		Model(std::string path, Ref<MeshRegistry>MeshReg, Ref<Shader> shader);
		Model(Ref<Shader> shader, std::vector<Ref<Mesh>> meshes);
		~Model() {};

		inline void SetShader(Ref<Shader> shader) { m_Shader = shader; };
		inline Ref<Shader> GetShader() { return m_Shader; };

		inline std::string GetPath() const { return m_path; };  // Declared as const member function

		inline std::vector<Ref<Mesh>>* GetMeshes() { return &m_Meshes; };

		void Draw();
		void Draw(glm::mat4 transform);

	private:
		std::string m_path;
		std::vector<Ref<Mesh>> m_Meshes;
		Ref<Shader> m_Shader;

	};
}