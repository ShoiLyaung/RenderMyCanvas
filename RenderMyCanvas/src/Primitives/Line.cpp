#include "Line.h"
#include <iostream>

Line::Line(const glm::vec2& start, const glm::vec2& end)
    : Primitive2D(start), m_Start(start), m_End(end)
{
}

void Line::Draw() const
{
    // Implement the Bresenham or Midpoint algorithm here
    std::cout << "Drawing line from " << m_Start.x << ", " << m_Start.y << " to " << m_End.x << ", " << m_End.y << std::endl;
}