// Ellipse.h
#pragma once

#include <glm/glm.hpp>
#include "Primitive.h"

namespace RMC {

    class Ellipse : public Primitive2D {
    public:
        Ellipse(const glm::vec3& center, float majorAxisLength, float minorAxisLength, float rotationAngle = 0.0f);

        Ellipse(const glm::vec3& focus1, const glm::vec3& focus2, float majorAxisLength);

        void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const override;

    protected:
        glm::vec3 Center;
        float MajorAxisLength;
        float MinorAxisLength;
        glm::vec3 Focus1;
        glm::vec3 Focus2;
        float RotationAngle;
    };

} // namespace RMC
