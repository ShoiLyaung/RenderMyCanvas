// Algorithms/PolygonScanlineFill.cpp
#include "PolygonScanlineFill.h"
#include <algorithm>

namespace RMC
{

namespace PolygonScanlineFill
{

struct Edge
{
    float ymax;  // �ߵ���� y ֵ
    float x;     // ��ǰɨ������ߵĽ��� x ����
    float dx;    // б�ʵĵ��� 1/m
};

void FillPolygon(const std::vector<glm::vec2>& vertices, uint32_t* imageData,
                 uint32_t width, uint32_t height, uint32_t fillColor)
{
    if (vertices.empty())
        return;

    // �ҵ�����ε���С����� y ֵ
    float ymin = vertices[0].y;
    float ymax = vertices[0].y;
    for (const auto& v : vertices) {
        if (v.y < ymin)
            ymin = v.y;
        if (v.y > ymax)
            ymax = v.y;
    }

    int scanlineStart = static_cast<int>(std::ceil(ymin));
    int scanlineEnd = static_cast<int>(std::floor(ymax));

    // �����߱�ET��
    std::vector<std::vector<Edge>> edgeTable(scanlineEnd - scanlineStart + 1);
    size_t vertexCount = vertices.size();
    for (size_t i = 0; i < vertexCount; ++i) {
        const glm::vec2& v1 = vertices[i];
        const glm::vec2& v2 = vertices[(i + 1) % vertexCount];

        if (v1.y == v2.y)
            continue;  // ˮƽ�ߣ�����

        Edge edge;
        if (v1.y < v2.y) {
            edge.ymax = v2.y;
            edge.x = v1.x;
            edge.dx = (v2.x - v1.x) / (v2.y - v1.y);
            edgeTable[static_cast<int>(std::ceil(v1.y)) - scanlineStart]
                .push_back(edge);
        } else {
            edge.ymax = v1.y;
            edge.x = v2.x;
            edge.dx = (v1.x - v2.x) / (v1.y - v2.y);
            edgeTable[static_cast<int>(std::ceil(v2.y)) - scanlineStart]
                .push_back(edge);
        }
    }

    // ��߱�AET��
    std::vector<Edge> activeEdgeTable;

    // ɨ���ߴ���
    for (int y = scanlineStart; y <= scanlineEnd; ++y) {
        // ���� AET��ɾ����ʧЧ�ı�
        activeEdgeTable.erase(
            std::remove_if(activeEdgeTable.begin(), activeEdgeTable.end(),
                           [y](const Edge& e) { return e.ymax <= y; }),
            activeEdgeTable.end());

        // ���µı߼��� AET
        const auto& edges = edgeTable[y - scanlineStart];
        activeEdgeTable.insert(activeEdgeTable.end(), edges.begin(),
                               edges.end());

        // �� x ��������
        std::sort(activeEdgeTable.begin(), activeEdgeTable.end(),
                  [](const Edge& a, const Edge& b) { return a.x < b.x; });

        // ���߶�
        for (size_t i = 0; i + 1 < activeEdgeTable.size(); i += 2) {
            int xStart = static_cast<int>(std::ceil(activeEdgeTable[i].x));
            int xEnd = static_cast<int>(std::floor(activeEdgeTable[i + 1].x));

            for (int x = xStart; x <= xEnd; ++x) {
                if (x >= 0 && x < static_cast<int>(width) && y >= 0 &&
                    y < static_cast<int>(height)) {
                    imageData[y * width + x] = fillColor;
                }
            }
        }

        // ���� x ֵ
        for (auto& edge : activeEdgeTable) {
            edge.x += edge.dx;
        }
    }
}

}  // namespace PolygonScanlineFill

}  // namespace RMC
