#include "DrawBoard.h"
#include <iostream>

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

void DrawBoard::Render(const Camera& camera)
{
    // Clear the image
    std::memset(m_ImageData, 0, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(uint32_t));

    // Render all primitives into m_ImageData
    for (const auto& primitive : m_Primitives)
    {
        primitive->Draw(m_ImageData, m_FinalImage->GetWidth(), m_FinalImage->GetHeight());
    }

    // Render the temporary primitive, if it exists
    if (m_TemporaryPrimitive)
    {
        m_TemporaryPrimitive->Draw(m_ImageData, m_FinalImage->GetWidth(), m_FinalImage->GetHeight());
    }

    // Set the final image data
    m_FinalImage->SetData(m_ImageData);
}

void DrawBoard::AddPrimitive(std::shared_ptr<Primitive> primitive) {
    m_Primitives.push_back(primitive);
}

void DrawBoard::SetTemporaryPrimitive(std::shared_ptr<Primitive> primitive) {
    m_TemporaryPrimitive = primitive;
}

void DrawBoard::ClearTemporaryPrimitive() {
    m_TemporaryPrimitive.reset();
}