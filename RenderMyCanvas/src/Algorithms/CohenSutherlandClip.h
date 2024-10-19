// Algorithms/CohenSutherlandClip.h
#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace RMC
{

namespace CohenSutherlandClip
{

/**
 * @brief ʹ�� Cohen-Sutherland �㷨�ü��߶�
 *
 * @param p0 �߶����
 * @param p1 �߶��յ�
 * @param xmin �ü����ڵ���С x
 * @param xmax �ü����ڵ���� x
 * @param ymin �ü����ڵ���С y
 * @param ymax �ü����ڵ���� y
 * @param clippedLines �ü�����߶��б�
 * @return true �߶��ڴ����ڻ򲿷��ڴ�����
 * @return false �߶���ȫ�ڴ�����
 */
bool ClipLine(const glm::vec2& p0, const glm::vec2& p1, float xmin, float xmax,
              float ymin, float ymax,
              std::vector<std::pair<glm::vec2, glm::vec2>>& clippedLines);

}  // namespace CohenSutherlandClip

}  // namespace RMC
