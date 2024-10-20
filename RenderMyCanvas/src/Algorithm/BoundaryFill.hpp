#pragma once
#include "BaseDrawAlgorithm.hpp"

namespace RMC::algorithm {
    class BoundaryFill
    {
    public:
        explicit BoundaryFill() {}

        virtual ~BoundaryFill() {}

        void SetParams(
            const glm::u64vec2& start,
            Ptr<std::vector<uint32_t>> imageBuffer,
            uint64_t imageWidth,
            uint64_t imageHeight,
            uint32_t fillColor
        ) {
            m_start = start;
            m_imageBuffer = imageBuffer;
            m_imageWidth = imageWidth;
            m_imageHeight = imageHeight;
            m_fillColor = fillColor;
            m_thisColor = (*m_imageBuffer)[start.y * m_imageWidth + start.x];
        }

        void Draw()
        {
            if (m_imageBuffer == nullptr
                || (*m_imageBuffer).at(m_start.y * m_imageWidth + m_start.x) == m_fillColor) {
                return;
            }
            std::stack<glm::u64vec2> stk;
            stk.push(m_start);
            while (!stk.empty()) {
                glm::u64vec2 coord = stk.top();
                uint64_t x = coord.x, y = coord.y;
                stk.pop();
                if (x >= m_imageWidth || y >= m_imageHeight) {
                    continue;
                }
                if ((*m_imageBuffer).at(y * m_imageWidth + x) != m_thisColor) {
                    continue;
                }
                (*m_imageBuffer).at(y * m_imageWidth + x) = m_fillColor;
                stk.push({ x + 1, y });
                stk.push({ x - 1, y });
                stk.push({ x, y + 1 });
                stk.push({ x, y - 1 });
            }
        }

    private:
        Ptr<std::vector<uint32_t>> m_imageBuffer = nullptr;
        uint64_t  m_imageWidth = 0;
        uint64_t  m_imageHeight = 0;
        glm::u64vec2 m_start;
        uint32_t  m_fillColor = 0;
        uint32_t  m_thisColor = 0;
    };
}