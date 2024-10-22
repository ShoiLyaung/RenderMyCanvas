#pragma once

#include <glm/glm.hpp>
#include "Ellipse.h"

namespace RMC {
    class Circle : public Primitive2D {
    public:
        Circle() = default;
        Circle(const glm::vec3& center, float radius, float lineWidth,
               uint32_t color);
        Circle(const glm::vec3& point1, const glm::vec3& point2,
               float lineWidth, uint32_t color);
        void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const override;
    protected:
        float Radius;
        glm::vec3 Center;
        float LineWidth;
        uint32_t Color;
    };
} // namespace RMC