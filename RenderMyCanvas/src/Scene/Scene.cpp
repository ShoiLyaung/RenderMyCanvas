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

		Material& mirrorMaterial = Materials.emplace_back();
		mirrorMaterial.Albedo = glm::vec3(0.9f, 0.9f, 0.9f);
		mirrorMaterial.Roughness = 0.0f;
		mirrorMaterial.Metallic = 1.0f;

		for (int i = 0; i < 3; i++) {
			Material& randomMaterial = Materials.emplace_back();
			randomMaterial.Albedo = predefinedColors[rand() % predefinedColors.size()];
			randomMaterial.Roughness = static_cast<float>(rand()) / RAND_MAX * 1.0f;
			randomMaterial.Metallic = 0.0f;
			randomMaterial.EmissionColor = randomMaterial.Albedo * 0.5f;
			randomMaterial.EmissionPower = 5.0f;
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
	}

	Scene::~Scene()
	{
	}

	bool Scene::OnUpdate(float ts, BallGame& ballGame)
	{
		Spheres.clear();
		for (const auto& player : ballGame.Players) {
			Spheres.push_back(std::make_unique<Player>(player));
		}

		for (const auto& item : ballGame.Foods) {
			Spheres.push_back(std::make_unique<Food>(item));
		}

		return 0;
	}
} // namespace RMC