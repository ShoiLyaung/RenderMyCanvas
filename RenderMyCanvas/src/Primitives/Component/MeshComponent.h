#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Primitives/Vertex.h"

namespace RMC {
    class SubMeshComponent {
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        std::vector<glm::vec3> Normals;
        std::vector<glm::vec2> TextureCoords;
    };
} // namespace RMC