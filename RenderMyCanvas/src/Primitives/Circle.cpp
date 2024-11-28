#include "Circle.h"
#include "Algorithms/CircleAlgorithm.h"

namespace RMC {

    Circle::Circle(const glm::vec3& center, float radius, float lineWidth,
               uint32_t color)
    : Radius(radius), Center(center), LineWidth(lineWidth), Color(color)
    {
    }

    Circle::Circle(const glm::vec3& point1, const glm::vec3& point2, float lineWidth,
                   uint32_t color)
        : LineWidth(lineWidth), Color(color)
	{
		Center = (point1 + point2) * 0.5f;
		Radius = glm::distance(point1, point2) * 0.5f;
	}

    void Circle::Draw(uint32_t* imageData, uint32_t width,
                      uint32_t height) const
    {
        CircleAlgorithm::MidPoint(Center, Radius, imageData, width, height,LineWidth,Color);
    }

} // namespace RMC