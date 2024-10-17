#include "Circle.h"
#include "Algorithms/CircleAlgorithm.h"

namespace RMC {

    Circle::Circle(const glm::vec3& center, float radius)
        : Radius(radius) 
    {
		Center = center;
    }

    Circle::Circle(const glm::vec3& point1, const glm::vec3& point2)
	{
		Center = (point1 + point2) * 0.5f;
		Radius = glm::distance(point1, point2) * 0.5f;
	}

    void Circle::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const {
        CircleAlgorithm::MidPoint(Center, Radius, imageData, width, height);
    }

} // namespace RMC