#pragma once

#include <glm/glm.hpp>
#include "Primitive.h"

namespace RMC {
    class Line : public Primitive2D {
    public:
        Line(const glm::vec3& start, const glm::vec3& end, float lineWidth,
             uint32_t color);
        void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const override;
    private:
        glm::vec3 Start;
        glm::vec3 End;
        float LineWidth;
        uint32_t Color;
    };
} // namespace RMC