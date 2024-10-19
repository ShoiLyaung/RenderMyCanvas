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
 * @brief ʹ��ɨ�����㷨�������
 *
 * @param vertices ����εĶ����б���˳ʱ�����ʱ��˳��
 * @param imageData ͼ�����ݻ�����
 * @param width ͼ����
 * @param height ͼ��߶�
 * @param fillColor �����ɫ��RGBA��ʽ��
 */
void FillPolygon(const std::vector<glm::vec2>& vertices, uint32_t* imageData,
                 uint32_t width, uint32_t height,
                 uint32_t fillColor = 0xFFFFFFFF);

}  // namespace PolygonScanlineFill

}  // namespace RMC
