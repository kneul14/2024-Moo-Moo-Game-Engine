#include "MooMooEngine_PCH.h"
#include "MeshRegistry.h"

namespace MooMooEngine
{
	std::vector<Ref<Mesh>> MeshRegistry::GetMesh(const std::string name)
	{
		std::string modelName = name.substr(name.find_last_of('/') + 1, name.find_last_of('.') - (name.find_last_of('/') + 1));

		if (m_meshesLoaded.find(modelName) != m_meshesLoaded.end())
		{
			return m_meshesLoaded[modelName];
		}
		Load(name);
		return m_meshesLoaded[modelName];
	}

	void MeshRegistry::AddMesh(const std::string& name, const Ref<Mesh>& mesh)
	{
		if (m_meshesLoaded.find(name) == m_meshesLoaded.end())
		{
			m_meshesLoaded[name].emplace_back(mesh);
		}
	}

	void MeshRegistry::AddTexture(const std::string& name, const Ref<Texture>& texture)
	{
		if (m_TexturesLoaded.find(name) == m_TexturesLoaded.end())
		{
			m_TexturesLoaded[name].emplace_back(texture);
		}
	}

	void MeshRegistry::Load(const std::string& path)
	{
		//m_meshesLoaded.emplace(path, new std::vector<Ref<Mesh>>());

		std::string directory = "";

		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('/'));
		std::string fileName = path.substr(path.find_last_of('/')+1, path.find_last_of('.')-(path.find_last_of('/') + 1));

		// process ASSIMP's root node recursively
		processNode(directory, fileName, scene->mRootNode, scene);

		textures_loaded.clear(); //MUST DO THIS //TODO Replace with more efficient approach
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void MeshRegistry::processNode(std::string directory, std::string fileName, aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(directory, fileName, mesh, scene);
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(directory, fileName, node->mChildren[i], scene);
		}

	}

	void MeshRegistry::processMesh(std::string directory, std::string fileName, aiMesh* mesh, const aiScene* scene)
	{
		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture*> textures;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
				// bi tangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		loadMaterialTextures(directory, fileName, material, aiTextureType_DIFFUSE, "texture_diffuse");
		
		// 2. specular maps
		loadMaterialTextures(directory, fileName, material, aiTextureType_SPECULAR, "texture_specular");

		// 3. normal maps
		loadMaterialTextures(directory, fileName, material, aiTextureType_HEIGHT, "texture_normal");

		// 4. height maps
		loadMaterialTextures(directory, fileName, material, aiTextureType_AMBIENT, "texture_height");

		//Add mesh to map
		AddMesh(fileName,std::make_shared<Mesh>(vertices, indices, textures));
	}

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	void MeshRegistry::loadMaterialTextures(std::string directory, std::string fileName, aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j]->GetPath().data(), str.C_Str()) == 0)
				{
					AddTexture(fileName,textures_loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Ref<Texture2D> texture = std::make_shared<Texture2D>(str.C_Str());
				AddTexture(fileName, texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
			}
		}
	}



}