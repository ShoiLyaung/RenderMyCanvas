#include "Line.h"
#include "Algorithms/LineAlgorithm.h"

namespace RMC {

    Line::Line(const glm::vec3& start, const glm::vec3& end, float lineWidth,
           uint32_t color)
        : Start(start), End(end), LineWidth(lineWidth), Color(color) {}

    void Line::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const
    {
        // Example: use Bresenham's algorithm to draw the line
        LineAlgorithm::Bresenham(Start, End, imageData, width, height,LineWidth,Color);
    }

} // namespace RMC