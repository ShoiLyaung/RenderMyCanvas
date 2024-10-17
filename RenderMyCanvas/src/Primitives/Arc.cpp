#include "Arc.h"
#include "Algorithms/ArcAlgorithm.h"

namespace RMC
{

// Constructor for circular arc
Arc::Arc(const glm::vec3& center, float radius, float startAngle,
         float endAngle)
    : Center(center),
      MajorAxisLength(radius),
      MinorAxisLength(radius),
      RotationAngle(0.0f),
      StartAngle(startAngle),
      EndAngle(endAngle)
{
}

// Constructor for elliptical arc
Arc::Arc(const glm::vec3& center, float majorAxisLength, float minorAxisLength,
         float rotationAngle, float startAngle, float endAngle)
    : Center(center),
      MajorAxisLength(majorAxisLength),
      MinorAxisLength(minorAxisLength),
      RotationAngle(rotationAngle),
      StartAngle(startAngle),
      EndAngle(endAngle)
{
}

void Arc::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const
{
    ArcAlgorithm::DrawArc(Center, MajorAxisLength, MinorAxisLength,
                          RotationAngle, StartAngle, EndAngle, imageData, width,
                          height);
}

}  // namespace RMC
