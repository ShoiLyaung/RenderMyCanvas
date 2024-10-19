#include "SutherlandHodgmanPolygonClip.h"

namespace RMC
{

namespace SutherlandHodgmanPolygonClip
{

bool Inside(const glm::vec2& p, const glm::vec2& cp1, const glm::vec2& cp2)
{
    return (cp2.x - cp1.x) * (p.y - cp1.y) > (cp2.y - cp1.y) * (p.x - cp1.x);
}

glm::vec2 Intersection(const glm::vec2& cp1, const glm::vec2& cp2,
                       const glm::vec2& s, const glm::vec2& e)
{
    float dcx = cp1.x - cp2.x;
    float dcy = cp1.y - cp2.y;
    float dpx = s.x - e.x;
    float dpy = s.y - e.y;
    float n1 = cp1.x * cp2.y - cp1.y * cp2.x;
    float n2 = s.x * e.y - s.y * e.x;
    float n3 = dcx * dpy - dcy * dpx;
    return glm::vec2((n1 * dpx - dcx * n2) / n3, (n1 * dpy - dcy * n2) / n3);
}

void ClipPolygon(const std::vector<glm::vec2>& subjectPolygon,
                 const std::vector<glm::vec2>& clipPolygon,
                 std::vector<glm::vec2>& outputPolygon)
{
    outputPolygon = subjectPolygon;

    size_t clipVertexCount = clipPolygon.size();
    for (size_t i = 0; i < clipVertexCount; ++i) {
        std::vector<glm::vec2> inputList = outputPolygon;
        outputPolygon.clear();

        glm::vec2 cp1 = clipPolygon[i];
        glm::vec2 cp2 = clipPolygon[(i + 1) % clipVertexCount];

        size_t inputCount = inputList.size();
        for (size_t j = 0; j < inputCount; ++j) {
            glm::vec2 s = inputList[j];
            glm::vec2 e = inputList[(j + 1) % inputCount];

            if (Inside(e, cp1, cp2)) {
                if (!Inside(s, cp1, cp2)) {
                    outputPolygon.push_back(Intersection(cp1, cp2, s, e));
                }
                outputPolygon.push_back(e);
            } else if (Inside(s, cp1, cp2)) {
                outputPolygon.push_back(Intersection(cp1, cp2, s, e));
            }
        }
    }
}

}  // namespace SutherlandHodgmanPolygonClip

}  // namespace RMC
