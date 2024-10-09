#include "Line.h"
#include <iostream>

Line::Line(const glm::vec2& start, const glm::vec2& end)
    : Primitive2D(start), m_Start(start), m_End(end)
{
}

void Line::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const
{
    int x0 = static_cast<int>(m_Start.x);
    int y0 = static_cast<int>(m_Start.y);
    int x1 = static_cast<int>(m_End.x);
    int y1 = static_cast<int>(m_End.y);

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        if (x0 >= 0 && x0 < static_cast<int>(width) && y0 >= 0 && y0 < static_cast<int>(height))
        {
            imageData[y0 * width + x0] = 0xffffffff; // Set pixel to white
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}