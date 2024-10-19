#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace RMC
{

namespace SutherlandHodgmanPolygonClip
{

/**
 * @brief 使用 Sutherland-Hodgman 算法裁剪多边形
 *
 * @param subjectPolygon 需要裁剪的多边形顶点列表
 * @param clipPolygon 裁剪窗口的顶点列表（矩形窗口）
 * @param outputPolygon 裁剪后的多边形顶点列表
 */
void ClipPolygon(const std::vector<glm::vec2>& subjectPolygon,
                 const std::vector<glm::vec2>& clipPolygon,
                 std::vector<glm::vec2>& outputPolygon);

}  // namespace SutherlandHodgmanPolygonClip

}  // namespace RMC
