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
			Material& pinkSphere = m_Scene.Materials.emplace_back();
			pinkSphere.Albedo = { 1.0f, 0.0f, 1.0f };
			pinkSphere.Roughness = 0.0f;

			Material& blueSphere = m_Scene.Materials.emplace_back();
			blueSphere.Albedo = { 0.2f, 0.3f, 1.0f };
			blueSphere.Roughness = 0.1f;

			Material& orangeSphere = m_Scene.Materials.emplace_back();
			orangeSphere.Albedo = { 0.8f, 0.5f, 0.2f };
			orangeSphere.Roughness = 0.1f;
			orangeSphere.EmissionColor = orangeSphere.Albedo;
			orangeSphere.EmissionPower = 2.0f;

			{
				Sphere sphere;
				sphere.Position = { 0.0f, 0.0f, 0.0f };
				sphere.Radius = 1.0f;
				sphere.MaterialIndex = 0;
				m_Scene.Spheres.push_back(sphere);
			}

			{
				Sphere sphere;
				sphere.Position = { 2.0f, 0.0f, 0.0f };
				sphere.Radius = 1.0f;
				sphere.MaterialIndex = 2;
				m_Scene.Spheres.push_back(sphere);
			}

			{
				Sphere sphere;
				sphere.Position = { 0.0f, -101.0f, 0.0f };
				sphere.Radius = 100.0f;
				sphere.MaterialIndex = 1;
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
