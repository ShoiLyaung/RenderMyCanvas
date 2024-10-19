#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

namespace RMC
{

namespace ContourExtraction
{

/**
 * @brief 从图像数据中提取包含指定点的多边形轮廓
 *
 * @param imageData 图像数据缓冲区
 * @param width 图像宽度
 * @param height 图像高度
 * @param point 用户点击的点
 * @param vertices 输出的多边形顶点列表
 * @param edgeColor 边缘颜色（可选，如果已知多边形的边缘颜色）
 * @return true 成功提取到轮廓
 * @return false 未能提取到轮廓
 */
bool ExtractContour(uint32_t* imageData, uint32_t width, uint32_t height,
                    const glm::vec3& point, std::vector<glm::vec2>& vertices,
                    uint32_t edgeColor = 0xFF000000);

}  // namespace ContourExtraction

}  // namespace RMC
