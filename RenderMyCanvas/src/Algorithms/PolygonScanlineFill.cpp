// Algorithms/PolygonScanlineFill.cpp
#include "PolygonScanlineFill.h"
#include <algorithm>

namespace RMC
{

namespace PolygonScanlineFill
{

struct Edge
{
    float ymax;  // 边的最大 y 值
    float x;     // 当前扫描线与边的交点 x 坐标
    float dx;    // 斜率的倒数 1/m
};

void FillPolygon(const std::vector<glm::vec2>& vertices, uint32_t* imageData,
                 uint32_t width, uint32_t height, uint32_t fillColor)
{
    if (vertices.empty())
        return;

    // 找到多边形的最小和最大 y 值
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

    // 构建边表（ET）
    std::vector<std::vector<Edge>> edgeTable(scanlineEnd - scanlineStart + 1);
    size_t vertexCount = vertices.size();
    for (size_t i = 0; i < vertexCount; ++i) {
        const glm::vec2& v1 = vertices[i];
        const glm::vec2& v2 = vertices[(i + 1) % vertexCount];

        if (v1.y == v2.y)
            continue;  // 水平边，跳过

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

    // 活动边表（AET）
    std::vector<Edge> activeEdgeTable;

    // 扫描线处理
    for (int y = scanlineStart; y <= scanlineEnd; ++y) {
        // 更新 AET，删除已失效的边
        activeEdgeTable.erase(
            std::remove_if(activeEdgeTable.begin(), activeEdgeTable.end(),
                           [y](const Edge& e) { return e.ymax <= y; }),
            activeEdgeTable.end());

        // 将新的边加入 AET
        const auto& edges = edgeTable[y - scanlineStart];
        activeEdgeTable.insert(activeEdgeTable.end(), edges.begin(),
                               edges.end());

        // 按 x 坐标排序
        std::sort(activeEdgeTable.begin(), activeEdgeTable.end(),
                  [](const Edge& a, const Edge& b) { return a.x < b.x; });

        // 画线段
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

        // 更新 x 值
        for (auto& edge : activeEdgeTable) {
            edge.x += edge.dx;
        }
    }
}

}  // namespace PolygonScanlineFill

}  // namespace RMC
