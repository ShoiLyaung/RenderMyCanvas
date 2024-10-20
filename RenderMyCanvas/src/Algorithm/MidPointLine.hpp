#pragma once
#include "BaseDraw2D.hpp"
#include "CohenSutherland.hpp"
#include "MidPointSubdivision.hpp"

namespace RMC::algorithm {
    class MidPointLine : public BaseDraw2D
    {
    public:
        using BaseDraw2D::Point;
        using BaseDraw2D::uint64_t;
        using BaseDraw2D::ParamMap;
        using int64_t = long long;

    public:
        explicit MidPointLine() = default;

        virtual void SetParams(const ParamMap& params) override final
        {
            auto startPoint = params.find("start_point");
            if (startPoint != params.end()) {
                m_start = std::any_cast<Point>(startPoint->second);
            }
            auto endPoint = params.find("end_point");
            if (endPoint != params.end()) {
                m_end = std::any_cast<Point>(endPoint->second);
            }
            auto width = params.find("image_width");
            if (width != params.end()) {
                m_width = std::any_cast<float>(width->second);
            }
            auto height = params.find("image_height");
            if (height != params.end()) {
                m_height = std::any_cast<float>(height->second);
            }             auto mode = params.find("mode");
            if (mode != params.end()) {
                m_mode = std::any_cast<uint8_t>(mode->second);
            }
            auto transform = params.find("transform_func");
            if (transform != params.end()) {
                m_transformFunc = std::any_cast<std::function<Point(const Point&)>>(transform->second);
            }
            auto inverseTransform = params.find("inverse_transform_func");
            if (inverseTransform != params.end()) {
                m_inverseTransform = std::any_cast<std::function<Point(const Point&)>>(inverseTransform->second);
            }            auto putPoint = params.find("put_point_func");
            if (putPoint != params.end()) {
                m_putPointFunc = std::any_cast<std::function<void(const Point&)>>(putPoint->second);
            }
            auto [clippedStart, clippedEnd] = CohenSutherland::LineClip(
                m_start, m_end, { 0.0F, 0.0F }, { m_width, m_height }
            );

            m_start = m_transformFunc(clippedStart);
            m_end = m_transformFunc(clippedEnd);
        }

        virtual void Draw() const override final
        {
            auto [x0, y0, x1, y1] = utils::Round(m_start[0], m_start[1], m_end[0], m_end[1]);
            int32_t a = y0 - y1, b = x1 - x0, d = a + a + b, d1 = a + a, d2 = a + a + b + b;
            int32_t x = x0, y = y0;
            m_putPointFunc(m_inverseTransform(Point{ float(x), float(y) }));
            while (x < m_end[0]) {
                if (d < 0) {
                    x += 1;
                    y += 1;
                    d += d2;
                } else {
                    x += 1;
                    d += d1;
                }
                m_putPointFunc(m_inverseTransform(Point{ float(x), float(y) }));
            }
        }

    private:
        Point                              m_start;
        Point                              m_end;
        uint8_t                              m_mode;
        float                              m_width;
        float                              m_height;
        std::function<Point(const Point&)> m_transformFunc;
        std::function<Point(const Point&)> m_inverseTransform;
        std::function<void(const Point&)>  m_putPointFunc = [](const Point&) {};
    };
} // namespace::RMC::algorithm