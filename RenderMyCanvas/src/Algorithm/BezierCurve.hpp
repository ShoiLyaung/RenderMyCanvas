#pragma once
#include "BaseDraw2D.hpp"

namespace RMC::algorithm {

    class BezierCurve : public BaseDraw2D
    {
    public:
        using BaseDraw2D::Point;
        using BaseDraw2D::uint64_t;
        using BaseDraw2D::ParamMap;

    public:
        explicit BezierCurve() = default;

        virtual ~BezierCurve() {}

        virtual void SetParams(const ParamMap& params) override final
        {
            auto ctrlPoints = params.find("control_points");
            if (ctrlPoints != params.end()) {
                m_ctrlPoints = std::any_cast<std::vector<Point>>(ctrlPoints->second);
            }
            auto step = params.find("step");
            if (step != params.end()) {
                m_drawStep = std::any_cast<float>(step->second);
            }
            auto putPointFunc = params.find("put_point_func");
            if (putPointFunc != params.end()) {
                m_putPointFunc = std::any_cast<std::function<void(const Point&)>>(putPointFunc->second);
            }
        }

        virtual void Draw() const override final
        {
            std::vector<float> ts;
            for (float t = 0; t <= 1; t += m_drawStep) {
                ts.push_back(t);
            }
            if (m_ctrlPoints.size() < 2) { return; }
            for (auto t : ts) {
                for (uint64_t i = 0; i < m_ctrlPoints.size(); ++i) {
                    m_putPointFunc(CalculateBezierPoint(m_ctrlPoints, t));
                }
            }
        }

    private:

        Point CalculateBezierPoint(const std::vector<Point>& points, float u) const
        {
            uint64_t n = points.size();
            std::vector<Point> bezierPoint(n - 1);
            for (uint64_t i = 0; i < n - 1; i++) {
                bezierPoint[i][0] = (1 - u) * points[i][0] + u * points[i + 1][0];
                bezierPoint[i][1] = (1 - u) * points[i][1] + u * points[i + 1][1];
            }
            if (n == 2)
                return bezierPoint[0];
            else
                return CalculateBezierPoint(bezierPoint, u);
        }


        std::vector<Point>                m_ctrlPoints = {};
        float                             m_drawStep = 0.001f;
        std::function<void(const Point&)> m_putPointFunc = [](const Point&) {};
    };
}
