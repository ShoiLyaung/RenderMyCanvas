// Algorithms/MidpointSubdivisionClip.cpp
#include "MidpointSubdivisionClip.h"

namespace RMC
{

namespace MidpointSubdivisionClip
{

bool IsInside(const glm::vec2& p, float xmin, float xmax, float ymin,
              float ymax)
{
    return (p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax);
}

bool IsOutside(const glm::vec2& p0, const glm::vec2& p1, float xmin, float xmax,
               float ymin, float ymax)
{
    if ((p0.x < xmin && p1.x < xmin) || (p0.x > xmax && p1.x > xmax) ||
        (p0.y < ymin && p1.y < ymin) || (p0.y > ymax && p1.y > ymax)) {
        return true;
    }
    return false;
}

bool ClipLineRecursive(
    const glm::vec2& p0, const glm::vec2& p1, float xmin, float xmax,
    float ymin, float ymax,
    std::vector<std::pair<glm::vec2, glm::vec2>>& clippedLines, int depth,
    int maxDepth)
{
    if (IsOutside(p0, p1, xmin, xmax, ymin, ymax)) {
        return false;
    }

    if (IsInside(p0, xmin, xmax, ymin, ymax) &&
        IsInside(p1, xmin, xmax, ymin, ymax)) {
        clippedLines.push_back({p0, p1});
        return true;
    }

    if (depth >= maxDepth) {
        return false;
    }

    glm::vec2 mid = (p0 + p1) * 0.5f;
    bool result1 = ClipLineRecursive(p0, mid, xmin, xmax, ymin, ymax,
                                     clippedLines, depth + 1, maxDepth);
    bool result2 = ClipLineRecursive(mid, p1, xmin, xmax, ymin, ymax,
                                     clippedLines, depth + 1, maxDepth);

    return result1 || result2;
}

bool ClipLine(const glm::vec2& p0, const glm::vec2& p1, float xmin, float xmax,
              float ymin, float ymax,
              std::vector<std::pair<glm::vec2, glm::vec2>>& clippedLines,
              int maxDepth)
{
    return ClipLineRecursive(p0, p1, xmin, xmax, ymin, ymax, clippedLines, 0,
                             maxDepth);
}

}  // namespace MidpointSubdivisionClip

}  // namespace RMC
