// Algorithms/PolygonScanlineFill.h
#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

namespace RMC
{

namespace PolygonScanlineFill
{

/**
 * @brief 使用扫描线算法填充多边形
 *
 * @param vertices 多边形的顶点列表（按顺时针或逆时针顺序）
 * @param imageData 图像数据缓冲区
 * @param width 图像宽度
 * @param height 图像高度
 * @param fillColor 填充颜色（RGBA格式）
 */
void FillPolygon(const std::vector<glm::vec2>& vertices, uint32_t* imageData,
                 uint32_t width, uint32_t height,
                 uint32_t fillColor = 0xFFFFFFFF);

}  // namespace PolygonScanlineFill

}  // namespace RMC
