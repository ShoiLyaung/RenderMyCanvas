#pragma once
#include "BaseDraw2D.hpp"
#include "CohenSutherland.hpp"
#include "MidPointSubdivision.hpp"

namespace RMC::algorithm {
    class Bresenham : public BaseDraw2D
    {
    public:
        using BaseDraw2D::Point;
        using BaseDraw2D::uint64_t;
        using BaseDraw2D::ParamMap;
        using int64_t = long long;
        using uint8_t = unsigned char;

    public:
        explicit Bresenham() = default;

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
            }

            auto mode = params.find("mode");
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
            }

            auto putPoint = params.find("put_point_func");
            if (putPoint != params.end()) {
                m_putPointFunc = std::any_cast<std::function<void(const Point&)>>(putPoint->second);
            }

            if (m_transformFunc == nullptr || m_inverseTransform == nullptr) {
                return;
            }

            auto [clippedStart, clippedEnd] = CohenSutherland::LineClip(
                m_start, m_end, { 0.0F, 0.0F }, { m_width, m_height }
            );

            m_start = m_transformFunc(clippedStart);
            m_end = m_transformFunc(clippedEnd);
        }

        virtual void Draw() const override final
        {
            // Suppose that the line is from left to right
            auto [x, y, dx, dy] = utils::Round(
                m_start[0], m_start[1], m_end[0] - m_start[0], m_end[1] - m_start[1]
            );

            int64_t e = -dx;
            for (int64_t i = 0LL; i <= dx; i++)
            {
                if (m_mode == 0) {
                    m_putPointFunc(m_inverseTransform({ float(x), float(y) }));
                } else if (m_mode == 1) {
                    if (!(i % 30 >= 20 && i % 30 < 30)) {
                        m_putPointFunc(m_inverseTransform({ float(x), float(y) }));
                    }
                } else if (m_mode == 2) {
                    if (!(i % 10 >= 5 && i % 10 < 10)) {
                        m_putPointFunc(m_inverseTransform({ float(x), float(y) }));
                    }
                }
                x = x + 1LL;
                e = e + 2LL * dy;
                if (e >= 0LL)
                {
                    y++, e = e - 2LL * dx;
                }
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
}