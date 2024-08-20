#include "MooMooEngine_PCH.h"
#include "Model.h"

#include "glad/glad.h"

namespace MooMooEngine
{
	Model::Model(Ref<Shader> shader, std::vector<Ref<Mesh>> meshes)
	{
		m_Shader = shader;
		m_Meshes = meshes;
	}

	Model::Model(std::string path, Ref<MeshRegistry> meshReg, Ref<Shader> shader)
	{
		m_path = path;
		m_Shader = shader;
		m_Meshes = meshReg->GetMesh(path);
	}

	void Model::Draw()
	{
		Draw(glm::mat4(1.0f));
	}

	void Model::Draw(glm::mat4 transform)
	{
		m_Shader->SetMat4("u_Transform", transform);
		m_Shader->Bind();

		for (std::vector<Ref<Mesh>>::iterator it = m_Meshes.begin(); it != m_Meshes.end(); ++it) {

			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < it->get()->GetTextures().size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
				std::string number;
				std::string name = it->get()->GetTextures().at(i)->GetType();
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++); // transfer unsigned int to string
				else if (name == "texture_normal")
					number = std::to_string(normalNr++); // transfer unsigned int to string
				else if (name == "texture_height")
					number = std::to_string(heightNr++); // transfer unsigned int to string

				// now set the sampler to the correct texture unit
				m_Shader->SetInt((name + number).c_str(), i);

				// and finally bind the texture
				glBindTexture(GL_TEXTURE_2D, it->get()->GetTextures().at(i)->GetRendererID());
			}

			// draw mesh
			glBindVertexArray(it->get()->GetVAO());
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(it->get()->GetIBOSize()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// always good practice to set everything back to defaults once configured.
			glActiveTexture(GL_TEXTURE0);

		}

	}
}