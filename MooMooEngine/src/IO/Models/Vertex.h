#pragma once

#include <glm/glm.hpp>

namespace MooMooEngine {
#define MAX_BONE_INFLUENCE 4 

    struct Vertex {

        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;

        // Bone Indexes
        int m_BoneIDs[MAX_BONE_INFLUENCE];

        float m_Weights[MAX_BONE_INFLUENCE];
    };
}