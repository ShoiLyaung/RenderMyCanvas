#pragma once
#include "Utils/Utils.hpp"

namespace RMC::renderer {
    struct Ray
    {
        glm::vec3 origin;
        glm::vec3 direction;
    };
} // namespace RMC::renderer