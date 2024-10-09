#include "Circle.h"
#include <iostream>

Circle::Circle(const glm::vec2& center, float radius)
    : Primitive2D(center), m_Radius(radius)
{
}

void Circle::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const
{
    // Implement the midpoint algorithm for circle drawing here
    std::cout << "Drawing circle at " << m_Position.x << ", " << m_Position.y << " with radius " << m_Radius << std::endl;
}