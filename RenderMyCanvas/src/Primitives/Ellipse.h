#pragma once

#include <glm/glm.hpp>
#include "Primitive.h"

namespace RMC {
    class Ellipse : public Primitive2D {
    public:
		Ellipse() = default;
        Ellipse(const glm::vec3& F1, const glm::vec3& F2);
        void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const override;
    protected:
        glm::vec3 Center;
        glm::vec3 F1;
        glm::vec3 F2;
        float MajorAxis;
        float MinorAxis;
        float StartAngle;
        float EndAngle;
    };
} // namespace RMC