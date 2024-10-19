#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

namespace RMC
{

namespace ContourExtraction
{

/**
 * @brief ��ͼ����������ȡ����ָ����Ķ��������
 *
 * @param imageData ͼ�����ݻ�����
 * @param width ͼ����
 * @param height ͼ��߶�
 * @param point �û�����ĵ�
 * @param vertices ����Ķ���ζ����б�
 * @param edgeColor ��Ե��ɫ����ѡ�������֪����εı�Ե��ɫ��
 * @return true �ɹ���ȡ������
 * @return false δ����ȡ������
 */
bool ExtractContour(uint32_t* imageData, uint32_t width, uint32_t height,
                    const glm::vec3& point, std::vector<glm::vec2>& vertices,
                    uint32_t edgeColor = 0xFF000000);

}  // namespace ContourExtraction

}  // namespace RMC
