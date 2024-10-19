// Algorithms/MidpointSubdivisionClip.h
#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace RMC
{

namespace MidpointSubdivisionClip
{

/**
 * @brief 使用中点分割算法裁剪线段
 *
 * @param p0 线段起点
 * @param p1 线段终点
 * @param xmin 裁剪窗口的最小 x
 * @param xmax 裁剪窗口的最大 x
 * @param ymin 裁剪窗口的最小 y
 * @param ymax 裁剪窗口的最大 y
 * @param clippedLines 裁剪后的线段列表
 * @param maxDepth 最大递归深度（防止无限递归）
 * @return true 线段在窗口内或部分在窗口内
 * @return false 线段完全在窗口外
 */
bool ClipLine(const glm::vec2& p0, const glm::vec2& p1, float xmin, float xmax,
              float ymin, float ymax,
              std::vector<std::pair<glm::vec2, glm::vec2>>& clippedLines,
              int maxDepth = 10);

}  // namespace MidpointSubdivisionClip

}  // namespace RMC
