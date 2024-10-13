#include "Line.h"
#include "Algorithm/LineAlgorithm.h"

namespace RMC {

    Line::Line(const glm::vec3& start, const glm::vec3& end)
        : Start(start), End(end), LineWidth(1) {}

    void Line::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const {
        // Example: use Bresenham's algorithm to draw the line
        LineAlgorithm::Bresenham(Start, End, imageData, width, height);
    }

} // namespace RMC