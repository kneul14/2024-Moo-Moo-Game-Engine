#include "MooMooEngine_PCH.h"
#include "Renderer3D.h"
#include <cmath>

namespace MooMooEngine {

	Renderer3D::Renderer3D()
	{
		SetupTriangle();
		SetupSquare();
		SetupCircle();
		SetupCube();
		SetupSphere();
	}

	void Renderer3D::PreProcessing()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST); // some weird stuff was happening without this.
		glDisable(GL_BLEND);
	}

	void Renderer3D::RenderModel(Model model, glm::mat4 transform)
	{
		model.GetShader()->SetMat4("u_Transform", transform);
		model.GetShader()->Bind();

		glActiveTexture(GL_TEXTURE0); // Activate the texture unit first
		glBindTexture(GL_TEXTURE_2D, textureID);

		for (auto& mesh : *model.GetMeshes())
		{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < mesh->GetTextures().size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				std::string number;
				std::string name = mesh->GetTextures().at(i)->GetType();
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);
				else if (name == "texture_normal")
					number = std::to_string(normalNr++);
				else if (name == "texture_height")
					number = std::to_string(heightNr++);

				model.GetShader()->SetInt((name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, mesh->GetTextures().at(i)->GetRendererID());
			}

			glBindVertexArray(mesh->GetVAO());
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->GetIBOSize()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE0);
		}
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture when done
	}

    void Renderer3D::SetupTriangle() {
        glGenVertexArrays(1, &m_TriangleVAO);
        glBindVertexArray(m_TriangleVAO);

        // Define vertices (position and texture coordinates)
        float vertices[] = {
            // Position            // Normal           // TexCoords
            -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
             0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.5f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2
        };

        glGenBuffers(1, &m_TriangleVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // TexCoords attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &m_TriangleEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_TriangleEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Renderer3D::drawTriangle() {
        glBindVertexArray(m_TriangleVAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Renderer3D::SetupSquare() {
        glGenVertexArrays(1, &m_SquareVAO);
        glBindVertexArray(m_SquareVAO);

        float vertices[] = {
            // Position            // Normal           // TexCoords
            -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        glGenBuffers(1, &m_SquareVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_SquareVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // TexCoords attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &m_SquareEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SquareEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Renderer3D::drawSquare() {
        glBindVertexArray(m_SquareVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Renderer3D::SetupCircle() {
        const int segments = 64;
        const float PI = 3.14159265359f;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Center vertex
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f); // Normal vector for flat circle
        vertices.push_back(0.5f); // Center texture coordinate
        vertices.push_back(0.5f); // Center texture coordinate

        // Circle vertices
        for (int i = 0; i <= segments; ++i) {
            float angle = i * 2.0f * PI / segments;
            vertices.push_back(cos(angle) * 0.5f); // x
            vertices.push_back(sin(angle) * 0.5f); // y
            vertices.push_back(0.0f);              // z
            vertices.push_back(0.0f);              // nx
            vertices.push_back(0.0f);              // ny
            vertices.push_back(1.0f);              // nz
            vertices.push_back(cos(angle) * 0.5f + 0.5f); // u
            vertices.push_back(sin(angle) * 0.5f + 0.5f); // v
        }

        // Circle indices
        for (int i = 1; i <= segments; ++i) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        glGenVertexArrays(1, &m_CircleVAO);
        glBindVertexArray(m_CircleVAO);

        glGenBuffers(1, &m_CircleVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_CircleVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // TexCoords attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &m_CircleEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CircleEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Renderer3D::drawCircle() {
        glBindVertexArray(m_CircleVAO);
        glDrawElements(GL_TRIANGLES, 64 * 3, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Renderer3D::SetupCube() {
        glGenVertexArrays(1, &m_CubeVAO);
        glBindVertexArray(m_CubeVAO);

        float vertices[] = {
            // Position            // Normal           // TexCoords
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  1.0f,   1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  1.0f,   0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   0.0f, 0.0f,  1.0f,   0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,  1.0f,   1.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,   0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f,  0.0f,   1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,  0.0f,   1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,   0.0f, 1.0f,

             0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   1.0f, 0.0f,  0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  0.0f,   0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  0.0f,   1.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,

            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f,   0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  0.0f,   1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  0.0f,   0.0f, 0.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        glGenBuffers(1, &m_CubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // TexCoords attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &m_CubeEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Renderer3D::drawCube() {
        glBindVertexArray(m_CubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Renderer3D::SetupSphere() {
        const int X_SEGMENTS = 64;
        const int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        for (int y = 0; y <= Y_SEGMENTS; ++y) {
            for (int x = 0; x <= X_SEGMENTS; ++x) {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = cos(xSegment * 2.0f * PI) * sin(ySegment * PI);
                float yPos = cos(ySegment * PI);
                float zPos = sin(xSegment * 2.0f * PI) * sin(ySegment * PI);

                vertices.push_back(xPos * 0.5f);
                vertices.push_back(yPos * 0.5f);
                vertices.push_back(zPos * 0.5f);
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(zPos);
                vertices.push_back(xSegment);
                vertices.push_back(ySegment);
            }
        }

        bool oddRow = false;
        for (int y = 0; y < Y_SEGMENTS; ++y) {
            if (!oddRow) {
                for (int x = 0; x <= X_SEGMENTS; ++x) {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            } else {
                for (int x = X_SEGMENTS; x >= 0; --x) {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        glGenVertexArrays(1, &m_SphereVAO);
        glBindVertexArray(m_SphereVAO);

        glGenBuffers(1, &m_SphereVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_SphereVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // TexCoords attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &m_SphereEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Renderer3D::drawSphere() {
        glBindVertexArray(m_SphereVAO);
        glDrawElements(GL_TRIANGLE_STRIP, 64 * 64 * 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

}