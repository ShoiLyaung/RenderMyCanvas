#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Primitives/Primitive.h"

namespace RMC {
    struct SurfaceComponent {
        std::vector<std::vector<glm::vec3>> controlPoints;
        enum class SurfaceType { NURBS } type;
    };
} // namespace RMC