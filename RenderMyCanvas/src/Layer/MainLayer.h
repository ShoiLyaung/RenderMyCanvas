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
            std::vector<glm::vec3> predefinedColors = {
                {0.9f, 0.6f, 0.7f},
                {0.4f, 0.6f, 0.9f},
                {0.6f, 0.9f, 0.7f},
                {0.9f, 0.9f, 0.5f},
                {0.8f, 0.5f, 0.4f},
                {0.5f, 0.8f, 0.9f},
                {0.7f, 0.8f, 0.4f},
                {0.9f, 0.7f, 0.4f},
                {0.9f, 0.5f, 0.8f},
                {0.6f, 0.4f, 0.9f},
                {0.2f, 0.8f, 0.6f},
                {0.4f, 0.7f, 0.9f},
                {0.8f, 0.7f, 0.3f},
                {0.6f, 0.3f, 0.4f},
                {0.7f, 0.3f, 0.6f},
                {0.3f, 0.6f, 0.4f},
                {0.7f, 0.9f, 0.6f},
                {0.9f, 0.4f, 0.6f},
                {0.5f, 0.6f, 0.9f},
                {0.6f, 0.9f, 0.4f},
                {0.3f, 0.4f, 0.7f}
            };

            Material& floorMaterial = m_Scene.Materials.emplace_back();
            floorMaterial.Albedo = { 0.7f, 0.7f, 0.7f };
            floorMaterial.Roughness = 0.8f;
            floorMaterial.Metallic = 1.0f;

            Material& lightMaterial = m_Scene.Materials.emplace_back();
            lightMaterial.Albedo = { 1.0f, 1.0f, 1.0f };
            lightMaterial.EmissionColor = { 1.0f, 1.0f, 1.0f };
            lightMaterial.EmissionPower = 1.5f;

            Material& mirrorMaterial = m_Scene.Materials.emplace_back();
            mirrorMaterial.Albedo = { 0.99f, 0.99f, 0.99f };
            mirrorMaterial.Roughness = 0.0001f;
            mirrorMaterial.Metallic = 1.0f;
			mirrorMaterial.EmissionColor = { 0.0f, 0.0f, 0.0f };
			mirrorMaterial.EmissionPower = 0.0f;

            Material& glassMaterial = m_Scene.Materials.emplace_back();
            glassMaterial.Transparency = 1.0f;
            glassMaterial.IOR = 1.5f;

            Material& pinkSphere = m_Scene.Materials.emplace_back();
            pinkSphere.Albedo = { 0.9f, 0.6f, 0.7f };
            pinkSphere.Roughness = 0.3f;
			pinkSphere.Metallic = 0.0f;
			pinkSphere.EmissionColor = { 0.0f, 0.0f, 0.0f };
			pinkSphere.EmissionPower = 0.0f;

            Material& blueSphere = m_Scene.Materials.emplace_back();
            blueSphere.Albedo = { 0.4f, 0.6f, 0.9f };
            blueSphere.Roughness = 0.2f;
			blueSphere.Metallic = 0.0f;
			blueSphere.EmissionColor = { 0.0f, 0.0f, 0.0f };
			blueSphere.EmissionPower = 0.0f;

            Material& orangeSphere = m_Scene.Materials.emplace_back();
            orangeSphere.Albedo = { 0.8f, 0.5f, 0.4f };
            orangeSphere.Roughness = 0.1f;
            orangeSphere.EmissionColor = orangeSphere.Albedo * 0.5f;
            orangeSphere.EmissionPower = 1.5f;

            for (int i = 0; i < 8; i++) {
                Material& randomMaterial = m_Scene.Materials.emplace_back();
                randomMaterial.Albedo = predefinedColors[rand() % predefinedColors.size()];
                randomMaterial.Roughness = static_cast<float>(rand()) / RAND_MAX * 1.0f;
				randomMaterial.Metallic = 0.0f;
                if (rand() % 2 == 0) {
					randomMaterial.EmissionColor = randomMaterial.Albedo * 0.5f;
                    randomMaterial.EmissionPower = 1.5f;
                }
            }

			for (int i = 0; i < 6; i++) {
				Material& randomMaterial = m_Scene.Materials.emplace_back();
				randomMaterial.Albedo = predefinedColors[rand() % predefinedColors.size()];
				randomMaterial.Roughness = static_cast<float>(rand()) / RAND_MAX * 0.2f;
				randomMaterial.Metallic = static_cast<float>(rand()) / RAND_MAX * 0.2f + 0.8f;
			}

            for (int i = 0; i < 8; i++) {
                Material& randomMaterial = m_Scene.Materials.emplace_back();
                randomMaterial.Albedo = predefinedColors[rand() % predefinedColors.size()];
                randomMaterial.Roughness = static_cast<float>(rand()) / RAND_MAX * 1.0f;
                randomMaterial.Metallic = 0.0f;
            }

            int centralSpheresCount = 10;
            float baseRadius = 0.5f;
            float fixedGap = 0.05f;
            float currentYPosition = 0.0f;

            for (int i = 0; i < centralSpheresCount; i++) {
                Sphere sphere;
                sphere.Radius = baseRadius;
                if (i == 0)
                    currentYPosition = sphere.Radius;
                else
                    currentYPosition += sphere.Radius + baseRadius * 2 + fixedGap;
                sphere.Position = { currentYPosition, sphere.Radius, 0.0f };
                sphere.MaterialIndex = ((3 * static_cast<unsigned long long>(i)) % (m_Scene.Materials.size() - 1)) + 1;
                m_Scene.Spheres.push_back(sphere);
				baseRadius *= 1.2f;
            }

            {
                Sphere sphere;
                sphere.Position = { 0.0f, -1000.0f, 0.0f };
                sphere.Radius = 1000.0f;
                sphere.MaterialIndex = 0;
                m_Scene.Spheres.push_back(sphere);
            }
            {
                Sphere sphere;
                sphere.Position = { 0.0f, 0.0f, -1010.0f };
                sphere.Radius = 1000.0f;
                sphere.MaterialIndex = 2;
                m_Scene.Spheres.push_back(sphere);
            }
            {
                Sphere sphere;
                sphere.Position = { 0.0f, 0.0f, 1010.0f };
                sphere.Radius = 1000.0f;
                sphere.MaterialIndex = 2;
                m_Scene.Spheres.push_back(sphere);
            }

            for (int i = 0; i < 40; i++) {
                Sphere sphere;
                sphere.Position = {
                    static_cast<float>(rand()) / RAND_MAX * 20.0f - 20.0f,
                    static_cast<float>(rand()) / RAND_MAX * 10.0f,
                    static_cast<float>(rand()) / RAND_MAX * 10.0f - 5.0f
                };
                sphere.Radius = static_cast<float>(rand()) / RAND_MAX * 1.5f + 0.5f;
                sphere.MaterialIndex = rand() % m_Scene.Materials.size();
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
