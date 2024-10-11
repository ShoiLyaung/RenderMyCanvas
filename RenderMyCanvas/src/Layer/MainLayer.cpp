#include "MainLayer.h"

namespace RMC
{
        void MainLayer::OnUIRender()
    {
        RenderSettingWindow();
        RenderDrawingOptionsWindow();
        RenderViewport();
    }

    void MainLayer::RenderSettingWindow()
    {
        SettingWindow::Render(m_LastRenderTime, m_CurrentRenderer, m_DrawBoard, m_Renderer, m_Scene, m_Camera);
    }

    void MainLayer::RenderDrawingOptionsWindow()
    {
        DrawingOptionsWindow::Render(m_LastRenderTime, m_ToolManager, m_DrawBoard, m_Scene, m_Camera);
    }

    void MainLayer::RenderViewport()
    {
        ViewportWindow::Render(m_ViewportWidth, m_ViewportHeight, m_ViewportOffset, m_CurrentRenderer, m_LastRenderTime, m_Scene,m_Camera);
    }

    void MainLayer::OnUpdate(float ts)
    {
		m_Scene.OnUpdate(ts);
		m_Camera.OnUpdate(ts);

        // Handle mouse input for drawing in DrawBoard
        if (m_CurrentRenderer == &m_DrawBoard)
        {
            ImGuiIO& io = ImGui::GetIO();
            glm::vec2 mousePos = glm::vec2(io.MousePos.x - m_ViewportOffset.x, io.MousePos.y - m_ViewportOffset.y);
            // Limit drawing to inside the viewport bounds
            if (m_ToolManager != ToolManager::Tool::None && mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x < m_ViewportWidth && mousePos.y < m_ViewportHeight)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (io.MouseDown[0]) // Left mouse button is down
                {
                    if (!m_IsDrawing)
                    {
                        m_DrawStart = mousePos;
                        m_IsDrawing = true;
                    }
                    else
                    {
                        m_DrawBoard.SetTemporaryPrimitive(PrimitiveFactory::CreateLine(m_DrawStart, mousePos));
                    }
                }
                else if (m_IsDrawing)
                {
                    // Finalize the drawing
                    m_DrawBoard.AddPrimitive(PrimitiveFactory::CreateLine(m_DrawStart, mousePos));
                    m_DrawBoard.ClearTemporaryPrimitive();
                    m_IsDrawing = false;
                }
            }
        }
    }
} // namespace RMC