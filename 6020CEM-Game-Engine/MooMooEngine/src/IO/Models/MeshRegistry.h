#pragma once

#include "Mesh.h"
#include "Renderer/Textures/Texture2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace MooMooEngine {

	class MeshRegistry
	{
	public:
		// Uses the Fly Weight Pattern
		MeshRegistry() {};
		~MeshRegistry() {};

		void Load(const std::string& path);
		void AddMesh(const std::string& name, const Ref<Mesh>& mesh);
		void AddTexture(const std::string& name, const Ref<Texture>& texture);
		std::vector<Ref<Mesh>> GetMesh(const std::string name);

	private:
		void processNode(std::string directory, std::string fileName, aiNode* node, const aiScene* scene);
		void processMesh(std::string directory, std::string fileName, aiMesh* mesh, const aiScene* scene);

		void loadMaterialTextures(std::string directory, std::string fileName, aiMaterial* material, aiTextureType type, std::string typeName);

	private:
		std::unordered_map<std::string, std::vector<Ref<Mesh>> > m_meshesLoaded;
		std::unordered_map<std::string, std::vector<Ref<Texture>> > m_TexturesLoaded;

		std::vector<Ref<Texture>> textures_loaded;


	};
}

