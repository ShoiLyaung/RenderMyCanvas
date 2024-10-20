#pragma once

#include "Utils/Utils.hpp"
#include "Primitive/BasePrimitive.hpp"

namespace RMC::renderer {
    struct HitPayload
    {
        ~HitPayload() {}
        float                               hitDistance = 0;
        glm::vec3                             worldPosition{ 0 };
        glm::vec3                             worldNormal{ 0 };
        uint64_t                              objectIndex = 0;
        bool                                sphereOrPolygon = false;
    };
} // namespace RMC::renderer