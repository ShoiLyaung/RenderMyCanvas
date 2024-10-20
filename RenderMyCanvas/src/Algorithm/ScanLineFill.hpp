#pragma once

#include "AlgoPredefines.hpp"
#include "BaseDraw2D.hpp"
#include "SutherHodgman.hpp"
#pragma warning(disable : 4244)

namespace RMC::algorithm {
    struct Edge_ScanLineFill
    {
        using uint64_t = unsigned long long;
        uint64_t yMax; // The maximum y coordinate of the edge
        float x;   // The x coordinate of lowest edge point
        float dx;  // The inverse of the slope of the edge 
    };

    class EdgeTable_ScanLineFill
    {
    public:
        using Edge = Edge_ScanLineFill;
        using Point = Point<float, 2>;
        using uint64_t = unsigned long long;

    public:
        explicit EdgeTable_ScanLineFill() = default;
        ~EdgeTable_ScanLineFill() = default;


        void BuildET(const std::vector<Point>& vertice, uint64_t imageHeight)
        {
            table.clear();
            table.resize(imageHeight, {});
            for (uint64_t i = 0; i < vertice.size(); ++i) {
                uint64_t j = (i + 1) % vertice.size();
                yMin = std::min(yMin, uint64_t(vertice[i][1]));
                yMax = std::max(yMax, uint64_t(vertice[i][1]));
                if (vertice[i][1] == vertice[j][1]) {
                    continue;
                }
                Edge edge{};
                if (vertice[i][1] < vertice[j][1]) {
                    edge.x = vertice[i][0];
                    edge.yMax = vertice[j][1];
                    edge.dx = (vertice[j][0] - vertice[i][0]) / (vertice[j][1] - vertice[i][1]);
                    table[uint64_t(vertice[i][1])].push_back(edge);
                } else {
                    edge.x = vertice[j][0];
                    edge.yMax = vertice[i][1];
                    edge.dx = (vertice[j][0] - vertice[i][0]) / (vertice[j][1] - vertice[i][1]);
                    table[uint64_t(vertice[j][1])].push_back(edge);
                }
            }
        }

    public:
        uint64_t yMin = std::numeric_limits<uint64_t>::max();
        uint64_t yMax = 0ULL;

        std::vector<std::vector<Edge>> table;
    };


    class ScanLineFill : public BaseDraw2D
    {
    public:
        using BaseDraw2D::Point;
        using BaseDraw2D::uint64_t;
        using BaseDraw2D::ParamMap;
        using Edge = Edge_ScanLineFill;
        using EdgeTable = EdgeTable_ScanLineFill;

    public:
        explicit ScanLineFill() : BaseDraw2D() {}

        ~ScanLineFill() = default;

        virtual void SetParams(const ParamMap& params) override final
        {
            auto imageHeight = params.find("image_height");
            if (imageHeight != params.end()) {
                m_imageHeight = std::any_cast<float>(imageHeight->second);
            }
            auto imageWidth = params.find("image_width");
            if (imageWidth != params.end()) {
                m_imageWidth = std::any_cast<float>(imageWidth->second);
            }
            auto vertices = params.find("vertices");
            if (vertices != params.end()) {
                std::vector<Point> v = std::any_cast<std::vector<Point>>(vertices->second);

                std::vector<Point> b(
                    {
                        Point{0.0f,0.0f},
                        Point{0.0f, m_imageHeight - 1},
                        Point{m_imageWidth - 1, m_imageHeight - 1},
                        Point{m_imageWidth, 0.0f}
                    }
                );

                m_clipper.Clip(v, b);

                m_edgeTable.BuildET(v, m_imageHeight);
            }
            auto putPointFunc = params.find("put_point_func");
            if (putPointFunc != params.end()) {
                m_putPointFunc = std::any_cast<std::function<void(const Point&)>>(putPointFunc->second);
            }
        }

        virtual void Draw() const override final
        {
            m_activeEdgeList.clear();
            for (uint64_t y = m_edgeTable.yMin; y <= m_edgeTable.yMax; ++y) {
                for (auto& edge : m_edgeTable.table[y]) {
                    m_activeEdgeList.push_back(edge);
                }
                m_activeEdgeList.erase(
                    std::remove_if(
                        m_activeEdgeList.begin(),
                        m_activeEdgeList.end(),
                        [y](const Edge& edge) {
                            return edge.yMax == y;
                        }
                    ),
                    m_activeEdgeList.end()
                );
                std::ranges::sort(m_activeEdgeList, [](const Edge& lhs, const Edge& rhs) {
                    return lhs.x < rhs.x;
                    });
                for (uint64_t i = 0; i < m_activeEdgeList.size(); i += 2) {
                    for (uint64_t x = m_activeEdgeList[i].x; i < m_activeEdgeList.size() - 1 && x < m_activeEdgeList[i + 1].x; ++x) {
                        m_putPointFunc(Point{ float(x), float(y) });
                    }
                }
                for (auto& edge : m_activeEdgeList) {
                    edge.x += edge.dx;
                }
            }
        }



    private:
        float m_imageHeight = 0;
        float m_imageWidth = 0;
        mutable std::vector<Edge>         m_activeEdgeList{};
        EdgeTable_ScanLineFill            m_edgeTable{};
        std::function<void(const Point&)> m_putPointFunc = [](const Point&) {};

        SutherHodgman                     m_clipper{};
    };


} // namespace RMC::algorithm