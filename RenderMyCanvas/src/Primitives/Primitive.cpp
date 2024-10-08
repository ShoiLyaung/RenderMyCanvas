#include "Primitive.h"
#include "Line.h"
#include "Circle.h"

std::shared_ptr<Primitive> PrimitiveFactory::CreateLine(const glm::vec2& start, const glm::vec2& end)
{
    return std::make_shared<Line>(start, end);
}

std::shared_ptr<Primitive> PrimitiveFactory::CreateCircle(const glm::vec2& center, float radius)
{
    return std::make_shared<Circle>(center, radius);
}