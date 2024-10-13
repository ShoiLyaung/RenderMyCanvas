#pragma once
#include "Walnut/Application.h"

#include "Renderer/Renderer.h"
#include "DrawBoard/DrawBoard.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"

#include "ToolManager.h"
#include "SettingWindow.h"
#include "DrawingOptionsWindow.h"
#include "ViewportWindow.h"

namespace RMC
{
	class MainLayer : public Walnut::Layer
	{
	public:
        MainLayer()
            : m_Camera(45.0f, 0.1f, 100.0f),
                m_ToolManager(ToolManager::Tool::None),
                m_ViewportOffset(0.0f, 0.0f) 
		{
			{
				Sphere sphere;
				sphere.Position = { 0.0f, 0.0f, 0.0f };
				sphere.Radius = 0.5f;
				sphere.Albedo = { 1.0f, 0.0f, 1.0f };
				m_Scene.Spheres.push_back(sphere);
			}
			{
				Sphere sphere;
				sphere.Position = { 1.0f, 0.0f, -5.0f };
				sphere.Radius = 1.5f;
				sphere.Albedo = { 0.2f, 0.3f, 1.0f };
				m_Scene.Spheres.push_back(sphere);
			}
		}

		virtual void OnUIRender() override;
		virtual void OnUpdate(float ts) override;

	private:
		void RenderSettingWindow();
		void RenderDrawingOptionsWindow();
		void RenderViewport();

		Renderer m_Renderer;
		DrawBoard m_DrawBoard;
		Renderer* m_CurrentRenderer = &m_DrawBoard;
		Scene m_Scene;
		Camera m_Camera;

		ToolManager::Tool m_ToolManager;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		float m_LastRenderTime = 0.0f;
		glm::vec2 m_ViewportOffset;
	};
} // namespace RMC
