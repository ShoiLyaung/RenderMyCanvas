#pragma once

#include <glm/glm.hpp>

namespace RMC {
    struct TransformComponent {
        glm::mat4 transform = glm::mat4(1.0f);
    };
} // namespace RMC