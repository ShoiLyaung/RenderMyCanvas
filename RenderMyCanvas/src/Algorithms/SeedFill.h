// Algorithms/SeedFill.h
#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace RMC
{

namespace SeedFill
{

/**
 * @brief 使用种子填充算法填充区域（四连通）
 *
 * @param imageData 图像数据缓冲区
 * @param width 图像宽度
 * @param height 图像高度
 * @param fillColor 填充颜色
 * @param point 用户点击的点（种子点）
 */
void FillArea(uint32_t* imageData, uint32_t width, uint32_t height,
              uint32_t fillColor, const glm::vec3& point);

}  // namespace SeedFill

}  // namespace RMC
