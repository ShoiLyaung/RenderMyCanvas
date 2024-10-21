#include "Scene.h"

namespace RMC {
	Scene::Scene()
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

        Material& floorMaterial = Materials.emplace_back();
        floorMaterial.Albedo = { 0.7f, 0.7f, 0.7f };
        floorMaterial.Roughness = 0.8f;
        floorMaterial.Metallic = 1.0f;

        Material& lightMaterial = Materials.emplace_back();
        lightMaterial.Albedo = { 1.0f, 1.0f, 1.0f };
        lightMaterial.EmissionColor = { 1.0f, 1.0f, 1.0f };
        lightMaterial.EmissionPower = 1.5f;

        Material& mirrorMaterial = Materials.emplace_back();
        mirrorMaterial.Albedo = { 0.99f, 0.99f, 0.99f };
        mirrorMaterial.Roughness = 0.0001f;
        mirrorMaterial.Metallic = 1.0f;
        mirrorMaterial.EmissionColor = { 0.0f, 0.0f, 0.0f };
        mirrorMaterial.EmissionPower = 0.0f;

        Material& glassMaterial = Materials.emplace_back();
        glassMaterial.Transparency = 1.0f;
        glassMaterial.IOR = 1.5f;

        for (int i = 0; i < 3; i++) {
            Material& randomMaterial = Materials.emplace_back();
            randomMaterial.Albedo = predefinedColors[rand() % predefinedColors.size()];
            randomMaterial.Roughness = static_cast<float>(rand()) / RAND_MAX * 1.0f;
            randomMaterial.Metallic = 0.0f;
            if (rand() % 2 == 0) {
                randomMaterial.EmissionColor = randomMaterial.Albedo * 0.5f;
                randomMaterial.EmissionPower = 1.5f;
            }
        }

        for (int i = 0; i < 3; i++) {
            Material& randomMaterial = Materials.emplace_back();
            randomMaterial.Albedo = predefinedColors[rand() % predefinedColors.size()];
            randomMaterial.Roughness = static_cast<float>(rand()) / RAND_MAX * 0.2f;
            randomMaterial.Metallic = static_cast<float>(rand()) / RAND_MAX * 0.2f + 0.8f;
        }

        for (int i = 0; i < 4; i++) {
            Material& randomMaterial = Materials.emplace_back();
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
            sphere.MaterialIndex = ((3 * static_cast<unsigned long long>(i)) % (Materials.size() - 1)) + 1;
            Spheres.push_back(sphere);
            baseRadius *= 1.2f;
        }

        {
            Sphere sphere;
            sphere.Position = { 0.0f, -1000.0f, 0.0f };
            sphere.Radius = 1000.0f;
            sphere.MaterialIndex = 0;
            Spheres.push_back(sphere);
        }
        {
            Sphere sphere;
            sphere.Position = { 0.0f, 0.0f, -1010.0f };
            sphere.Radius = 1000.0f;
            sphere.MaterialIndex = 2;
            Spheres.push_back(sphere);
        }
        {
            Sphere sphere;
            sphere.Position = { 0.0f, 0.0f, 1010.0f };
            sphere.Radius = 1000.0f;
            sphere.MaterialIndex = 2;
            Spheres.push_back(sphere);
        }

        for (int i = 0; i < 40; i++) {
            Sphere sphere;
            sphere.Position = {
                static_cast<float>(rand()) / RAND_MAX * 20.0f - 20.0f,
                static_cast<float>(rand()) / RAND_MAX * 10.0f,
                static_cast<float>(rand()) / RAND_MAX * 10.0f - 5.0f
            };
            sphere.Radius = static_cast<float>(rand()) / RAND_MAX * 1.5f + 0.5f;
            sphere.MaterialIndex = rand() % Materials.size();
            Spheres.push_back(sphere);
        }
	}

	Scene::~Scene()
	{
	}

	//entt::entity Scene::CreateEntity()
	//{
	//	return m_Registry.create();
	//}

	bool Scene::OnUpdate(float ts)
	{
		return 0;
	}
} // namespace RMC