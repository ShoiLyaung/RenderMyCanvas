#pragma once

#include <glm/glm.hpp>

namespace RMC
{
    struct Vertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec3 Normal;
        glm::vec2 TextureCoords;
        glm::vec3 Tangent;
        glm::vec3 BiTangent;
    };
} // namespace RMC