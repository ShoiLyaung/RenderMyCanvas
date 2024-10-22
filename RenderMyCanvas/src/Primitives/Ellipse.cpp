// Ellipse.cpp
#include "Ellipse.h"
#include "Algorithms/EllipseAlgorithm.h"
#include <cmath>

namespace RMC {

    Ellipse::Ellipse(const glm::vec3& center, float majorAxisLength,
                 float minorAxisLength, float rotationAngle, float lineWidth,
                 uint32_t color)
    : Center(center),
      MajorAxisLength(majorAxisLength),
      MinorAxisLength(minorAxisLength),
      RotationAngle(rotationAngle),
      LineWidth(lineWidth),
      Color(color)
    {
        float c = std::sqrt(MajorAxisLength * MajorAxisLength - MinorAxisLength * MinorAxisLength);
        glm::vec2 direction(std::cos(RotationAngle), std::sin(RotationAngle));
        Focus1 = Center + glm::vec3(direction * c, 0.0f);
        Focus2 = Center - glm::vec3(direction * c, 0.0f);
    }

    Ellipse::Ellipse(const glm::vec3& focus1, const glm::vec3& focus2,
                     float majorAxisLength, float lineWidth, uint32_t color)
        : Focus1(focus1),
          Focus2(focus2),
          MajorAxisLength(majorAxisLength),
          LineWidth(lineWidth),
          Color(color)
    {
        Center = (Focus1 + Focus2) * 0.5f;
        float distanceBetweenFoci = glm::distance(Focus1, Focus2);
        float c = distanceBetweenFoci * 0.5f;
        MinorAxisLength = std::sqrt(MajorAxisLength * MajorAxisLength - c * c);
        RotationAngle = std::atan2(Focus2.y - Focus1.y, Focus2.x - Focus1.x);
    }

    void Ellipse::Draw(uint32_t* imageData, uint32_t width,
                       uint32_t height) const
    {
        EllipseAlgorithm::Midpoint(Center, MajorAxisLength, MinorAxisLength, RotationAngle, imageData, width, height,LineWidth,Color);
    }

    Ellipse_Bresenham::Ellipse_Bresenham(const glm::vec3& center,
                                         float majorAxisLength,
                                         float minorAxisLength,
                                         float rotationAngle, float lineWidth,
                                         uint32_t color)
		: Ellipse(center, majorAxisLength, minorAxisLength, rotationAngle,lineWidth,color)
	{
	}
    Ellipse_Bresenham::Ellipse_Bresenham(const glm::vec3& focus1,
                                         const glm::vec3& focus2,
                                         float majorAxisLength, float lineWidth,
                                         uint32_t color)
        : Ellipse(focus1, focus2, majorAxisLength,lineWidth,color)
    {}

    void Ellipse_Bresenham::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const {
        EllipseAlgorithm::Bresenham(Center, MajorAxisLength, MinorAxisLength, RotationAngle, imageData, width, height,LineWidth,Color);
    }

} // namespace RMC
