#pragma once

#include <glm/glm.hpp>
#include "Ellipse.h"

namespace RMC {
    class Circle : public Primitive2D {
    public:
        Circle() = default;
        Circle(const glm::vec3& center, float radius);
        void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const override;
    protected:
        float Radius;
        float MajorAxis;
        float MinorAxis;
        float StartAngle;
        float EndAngle;
        glm::vec3 Center;
    };
} // namespace RMC