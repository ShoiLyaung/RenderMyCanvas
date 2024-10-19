#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace RMC
{

namespace SutherlandHodgmanPolygonClip
{

/**
 * @brief ʹ�� Sutherland-Hodgman �㷨�ü������
 *
 * @param subjectPolygon ��Ҫ�ü��Ķ���ζ����б�
 * @param clipPolygon �ü����ڵĶ����б����δ��ڣ�
 * @param outputPolygon �ü���Ķ���ζ����б�
 */
void ClipPolygon(const std::vector<glm::vec2>& subjectPolygon,
                 const std::vector<glm::vec2>& clipPolygon,
                 std::vector<glm::vec2>& outputPolygon);

}  // namespace SutherlandHodgmanPolygonClip

}  // namespace RMC
