#include "DrawBoard.h"
#include <iostream>

DrawBoard::DrawBoard()
    : m_IsDrawing(false) {}

void DrawBoard::AddPrimitive(std::shared_ptr<Primitive> primitive) {
    m_Primitives.push_back(primitive);
}

void DrawBoard::OnMouseEvent(int button, int action, int x, int y) {
    if (button == 0 && action == 1) { // Example: Left mouse button down
        if (!m_IsDrawing) {
            m_TempPoint = glm::vec2(x, y);
            m_IsDrawing = true;
        }
        else {
            auto line = std::make_shared<Line>(m_TempPoint, glm::vec2(x, y));
            AddPrimitive(line);
            m_IsDrawing = false;
        }
    }
}

void DrawBoard::Render()
{
    // Render all primitives
    for (const auto& primitive : m_Primitives)
    {
        primitive->Draw();
    }
    //render every pixel

    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
    {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
        {
            glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
            m_ImageData[y * m_FinalImage->GetWidth() + x] = PerPixel(coord);
        }
    }
    m_FinalImage->SetData(m_ImageData);
}