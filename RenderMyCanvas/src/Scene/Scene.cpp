#include "Scene.h"

namespace RMC {
	Scene::Scene()
	{
		Material& floorMaterial = Materials.emplace_back();
		floorMaterial.Albedo = { 0.7f, 0.7f, 0.7f };
		floorMaterial.Roughness = 0.8f;
		floorMaterial.Metallic = 1.0f;

		//{
		//	Sphere sphere;
		//	sphere.Position = { 0.0f, -1000.0f, 0.0f };
		//	sphere.Radius = 1000.0f;
		//	sphere.MaterialIndex = 0;
		//	Spheres.push_back(sphere);
		//}
	}

	Scene::~Scene()
	{
	}

	bool Scene::OnUpdate(float ts)
	{
		return 0;
	}
} // namespace RMC