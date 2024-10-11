#pragma once

#include <glm/glm.hpp>

namespace RMC
{
    struct Vertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };
} // namespace RMC