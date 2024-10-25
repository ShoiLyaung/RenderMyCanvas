#pragma once
#include "Walnut/Application.h"

#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Systems/InputSystem.h"

#include "SettingWindow.h"
#include "ViewportWindow.h"

namespace RMC
{
	class MainLayer : public Walnut::Layer
	{
	public:
        MainLayer()
            : m_Camera(45.0f, 0.1f, 100.0f),
                m_ViewportOffset(0.0f, 0.0f) {}

		virtual void OnUIRender() override;
		virtual void OnUpdate(float ts) override;

	private:
		void RenderSettingWindow();
		void RenderViewport();

		Renderer m_Renderer;
		Renderer* m_CurrentRenderer = &m_Renderer;
		Scene m_Scene;
		Camera m_Camera;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		float m_LastRenderTime = 0.0f;
		glm::vec2 m_ViewportOffset;
	};
} // namespace RMC
