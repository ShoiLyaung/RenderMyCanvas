#pragma once

#include "Primitive.h"
#include <glm/glm.hpp>


namespace RMC
{

class Arc : public Primitive2D
{
public:
    // Constructor for circular arc
    Arc(const glm::vec3& center, float radius, float startAngle,
        float endAngle);

    // Constructor for elliptical arc
    Arc(const glm::vec3& center, float majorAxisLength, float minorAxisLength,
        float rotationAngle, float startAngle, float endAngle);

    void Draw(uint32_t* imageData, uint32_t width,
              uint32_t height) const override;

protected:
    glm::vec3 Center;
    float MajorAxisLength;  // Semi-major axis length (a)
    float MinorAxisLength;  // Semi-minor axis length (b)
    float RotationAngle;    // Rotation angle in radians
    float StartAngle;       // Start angle in radians
    float EndAngle;         // End angle in radians
};

}  // namespace RMC
