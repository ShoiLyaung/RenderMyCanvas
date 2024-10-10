#pragma once
#include "Walnut/Application.h"

#include "Renderer/Renderer.h"
#include "DrawBoard/DrawBoard.h"
#include "Camera/Camera.h"

#include "ToolManager.h"
#include "SettingWindow.h"
#include "DrawingOptionsWindow.h"
#include "ViewportWindow.h"

namespace RMC::layer {

	class MainLayer : public Walnut::Layer
	{
	public:
        MainLayer()
            : m_Camera(45.0f, 0.1f, 100.0f),
                m_DrawStart(0.0f, 0.0f),
                m_ToolManager(ToolManager::Tool::None),
                m_ViewportOffset(0.0f, 0.0f) {}

		virtual void OnUIRender() override;
		virtual void OnUpdate(float ts) override;

	private:
	private:
		void RenderSettingWindow();
		void RenderDrawingOptionsWindow();
		void RenderViewport();

		Renderer m_Renderer;
		DrawBoard m_DrawBoard;
		Renderer* m_CurrentRenderer = &m_DrawBoard;
		Camera m_Camera;

		ToolManager::Tool m_ToolManager;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		float m_LastRenderTime = 0.0f;
		bool m_IsDrawing = false;
		glm::vec2 m_DrawStart;
		glm::vec2 m_ViewportOffset;
	};
} // namespace RMC::layer
