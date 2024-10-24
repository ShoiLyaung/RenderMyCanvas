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
        if (m_Camera.OnUpdate(ts))
        {
			m_Renderer.ResetFrameIndex();
        }
        InputSystem::Update(m_Scene.GetRegistry(), ts);
        // Handle mouse input for drawing in DrawBoard
        if (m_CurrentRenderer == &m_DrawBoard)
        {
            ImGuiIO& io = ImGui::GetIO();
            glm::vec2 mousePos = glm::vec2(io.MousePos.x - m_ViewportOffset.x, io.MousePos.y - m_ViewportOffset.y);
            // Limit drawing to inside the viewport bounds
            if (m_ToolManager != ToolManager::Tool::None && mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x < m_ViewportWidth && mousePos.y < m_ViewportHeight)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				m_DrawBoard.OnMouseEvent(io.MouseDown[0], mousePos.x, mousePos.y);
            }
        }
    }
} // namespace RMC