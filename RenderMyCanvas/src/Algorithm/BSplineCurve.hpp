#pragma once
#include "BaseDraw2D.hpp"
#include "AlgoRandom.hpp"

namespace RMC::algorithm {

    class BSplineCurve : public BaseDraw2D
    {
    public:
        using BaseDraw2D::Point;
        using BaseDraw2D::uint64_t;
        using BaseDraw2D::ParamMap;

    public:
        explicit BSplineCurve() = default;

        virtual ~BSplineCurve() {}

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
            auto order = params.find("order");
            if (order != params.end()) {
                m_order = std::any_cast<uint64_t>(order->second);
            }
            auto putPointFunc = params.find("put_point_func");
            if (putPointFunc != params.end()) {
                m_putPointFunc = std::any_cast<std::function<void(const Point&)>>(putPointFunc->second);
            }
            GenerateKnots();
            GenerateWeights();
        }


        virtual void Draw() const override final
        {
            for (float u = 0; u <= 1.0; u += m_drawStep) {
                float denominator = 0.0F;
                Point p = { 0.0F, 0.0F };
                for(uint64_t i=0; i < m_ctrlPoints.size(); ++i) {
                    float weight =
                        m_weights[i] *
                        BasicFunction(i, static_cast<int>(m_order), u, m_knots);
                    denominator += weight;
                    p[0] += m_ctrlPoints[i][0] * weight;
                    p[1] += m_ctrlPoints[i][1] * weight;
                }
                if(denominator != 0.0F) {
                    p[0] /= denominator;
                    p[1] /= denominator;
                }
                m_putPointFunc(p);
            }
        }

    private:
        void GenerateKnots()
        {
            uint64_t num = m_ctrlPoints.size() + m_order;
            if (m_knots.size() != num) {
                m_knots.clear();
                // Generate a new random knot vector
                m_knots.resize(num, 0.0F);
                for (uint64_t i = num - 1; i >= num - m_order; --i) {
                    m_knots.at(i) = 1;
                }
                for (uint64_t i = m_order; i < num - m_order; ++i) {
                    // Make sure `m_knots[m_order:num-m_order-1]` is strictly increasing
                    float r = m_randGen(std::numeric_limits<float>::epsilon() * 2.0F, 1.0F - m_knots[i - 1]) * 0.5f;
                    m_knots[i] = r + m_knots[i-1];
                }
            }
        }

        void GenerateWeights()
        {
            uint64_t num = m_ctrlPoints.size();
            if (m_weights.size() != num) {
                m_weights.clear();
                m_weights.resize(num);
                for (auto& w : m_weights)
                {
                    w = m_randGen(15, 25);
                }
                float sum = std::accumulate(m_weights.begin(), m_weights.end(), 0.0F);
                for (auto& w : m_weights)
                {
                    w /= sum;
                }
            }
        }

        static float BasicFunction(uint64_t i, int order, float t, std::vector<float>& knots)
        {
            if (order == 1) {
                if (t >= knots[i] && t <= knots[i + 1ULL]) {
                    return 1.0F;
                }
                return 0.0F;
            }
            // Or k > 1, then the BSpline is a piecewise polynomial function
            else {
                float deltaA = knots[i + order - 1ULL] - knots[i];
                float deltaB = knots[i + order] - knots[i + 1ULL];
                float a = (t - knots[i]) / (deltaA == 0.0F ? 1.0F : deltaA);
                float b = (knots[i + order] - t) / (deltaB == 0.0F ? 1.0F : deltaB);
                return a * BasicFunction(i, order - 1, t, knots) + b * BasicFunction(i + 1ULL, order - 1, t, knots);
            }

        }

    private:
        // Parameters for BSplineCurve
        std::vector<Point>                m_ctrlPoints{};
        uint64_t                            m_order = 3ULL;
        float                             m_drawStep = 1e-4F;
        std::function<void(const Point&)> m_putPointFunc = [](const Point&) {};
        
        std::vector<float>                m_weights{};
        mutable std::vector<float>        m_knots{};
        Rand_Uniform<float>               m_randGen{};
    };
}
