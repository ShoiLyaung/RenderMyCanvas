// Algorithms/SeedFill.h
#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace RMC
{

namespace SeedFill
{

/**
 * @brief ʹ����������㷨�����������ͨ��
 *
 * @param imageData ͼ�����ݻ�����
 * @param width ͼ����
 * @param height ͼ��߶�
 * @param fillColor �����ɫ
 * @param point �û�����ĵ㣨���ӵ㣩
 */
void FillArea(uint32_t* imageData, uint32_t width, uint32_t height,
              uint32_t fillColor, const glm::vec3& point);

}  // namespace SeedFill

}  // namespace RMC
