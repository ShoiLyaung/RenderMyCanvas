#pragma once
#include "BaseDraw2D.hpp"
////#include "BaseDrawAlgorithm.hpp"

namespace RMC::algorithm {
    class MidPointEllipse : public BaseDraw2D
    {
    public:
        using BaseDraw2D::Point;
        using BaseDraw2D::uint64_t;
        using BaseDraw2D::ParamMap;

    public:
        explicit MidPointEllipse() : BaseDraw2D(), m_a(0), m_b(0) {}

        virtual void SetParams(const ParamMap& params) override final
        {
            auto a = params.find("a");
            if (a != params.end()) {
                m_a = int(std::any_cast<float>(a->second));
            } else {
                std::cout << std::format("[WARN] {} >>> Param: `a` not found", __FUNCTION__);
                m_a = 0;
            }
            auto b = params.find("b");
            if (b != params.end()) {
                m_b = int(std::any_cast<float>(b->second));
            } else {
                std::cout << std::format("[WARN] {} >>> Param: `b` not found", __FUNCTION__);
                m_b = 0;
            }
            auto transformFunc = params.find("transform_func");
            if (transformFunc != params.end()) {
                m_transformFunc = std::any_cast<std::function<Point(const Point&)>>(transformFunc->second);
            } else {
                std::cout << std::format("[WARN] {} >>> Param: `transform_func` not found", __FUNCTION__);
                m_transformFunc = [](const Point& p) { return p; };
            }
            auto putPointFunc = params.find("put_point_func");
            if (putPointFunc != params.end()) {
                m_putPointFunc = std::any_cast<std::function<void(const Point&)>>(putPointFunc->second);
            } else {
                std::cout << std::format("[WARN] {} >>> Param: `put_point_func` not found", __FUNCTION__);
                m_putPointFunc = [](const Point&) {};
            }
        }

        virtual void Draw() const override final
        {
            float d1 = 1.0F * (m_b * m_b - m_a * m_a * m_b) + 0.25F * m_a * m_a;
            int x = 0, y = m_b;
            m_putPointFunc(m_transformFunc({ float(x), float(y) }));
            // Upper part:
            while (m_b * m_b * (x + 1) < m_a * m_a * y - 0.5F * m_a * m_a) {
                if (d1 < 0) {
                    d1 += m_b * m_b * (2 * x + 3);
                    ++x;
                } else {
                    d1 += m_b * m_b * (2 * x + 3) + m_a * m_a * (-2 * y + 2);
                    ++x, --y;
                }
                m_putPointFunc(m_transformFunc({ float(x), float(y) }));
            }
            // Lower part:
            float d2 = float(::sqrt(m_b * x + 0.5F * m_b) + ::sqrt(m_a * (y - 1)) - ::sqrt(m_a * m_b));
            while (y > 0) {
                if (d2 < 0) {
                    d2 += m_b * m_b * (2 * x + 2) + m_a * m_a * (-2 * y + 3);
                    ++x, --y;
                } else {
                    d2 += m_a * m_a * (-2 * y + 3);
                    --y;
                }
                m_putPointFunc(m_transformFunc({ float(x), float(y) }));
            }
        }

    private:

        int m_a;
        int m_b;

        std::function<Point(const Point&)> m_transformFunc;
        std::function<void(const Point&)> m_putPointFunc = [](const Point&) {};
    };
}