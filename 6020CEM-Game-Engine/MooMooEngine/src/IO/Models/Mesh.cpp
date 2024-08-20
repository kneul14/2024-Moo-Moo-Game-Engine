#include "MooMooEngine_PCH.h"
#include "Mesh.h"

#include "Glad/glad.h"

using namespace std;

namespace MooMooEngine {
    Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture*> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        CreateBuffers();

        AssignData();
    }

    void Mesh::CreateBuffers() {
        glCreateVertexArrays(1, &m_VertexArray);
        glGenBuffers(1, &m_VertexBuffer);
        glGenBuffers(1, &m_IndexBuffer);
    }

    void Mesh::AssignData()
    {
        glBindVertexArray(m_VertexArray);

        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // Vertext texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));


        // Tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        // Bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        // Vertex Bone IDs
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // Vertex Bone Weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

        glBindVertexArray(0);
    }

    
}

