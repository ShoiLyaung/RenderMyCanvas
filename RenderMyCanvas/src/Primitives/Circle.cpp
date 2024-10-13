#include "Circle.h"
#include "Algorithm/CircleAlgorithm.h"

namespace RMC {

    Circle::Circle(const glm::vec3& center, float radius)
        : Radius(radius) 
    {
		Center = center;
		MajorAxis = radius;
		MinorAxis = radius;
		StartAngle = 0;
		EndAngle = 360;
    }

    void Circle::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const {
        // Example: use Midpoint algorithm to draw the circle
        CircleAlgorithm::MidPoint(Center, Radius, imageData, width, height);
    }

} // namespace RMC