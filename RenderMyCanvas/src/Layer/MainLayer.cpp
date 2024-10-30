#include "MainLayer.h"

namespace RMC
{
        void MainLayer::OnUIRender()
    {
        RenderSettingWindow();
        RenderViewport();
    }

    void MainLayer::RenderSettingWindow()
    {
        SettingWindow::Render(m_LastRenderTime, m_CurrentRenderer, m_Renderer, m_Scene, m_Camera);
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
    }
} // namespace RMC