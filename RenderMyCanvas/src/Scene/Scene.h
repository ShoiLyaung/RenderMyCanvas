#pragma once

#include <vector>
#include <glm/glm.hpp>
//#include "entt.hpp"

#include "Primitives/Sphere.h"
#include "Material/Material.h"

namespace RMC
{
	class Scene
	{
	public:
		struct Settings
		{
			bool night = false;
		};
		Scene() ;
		~Scene();
		//std::vector<Primitive> Primitives;
		std::vector<Sphere> Spheres;
		std::vector<int> playerID = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
		std::vector<Material> Materials;
		glm::vec3 GetSkyColor() const { return skyColor; }
		Settings& GetSettings() { return m_Settings; }
		bool IsNight() const { return m_Settings.night; }
		//entt::entity CreateEntity();

		bool OnUpdate(float ts);
	protected:
		glm::vec3 skyColor = glm::vec3(0.16f, 0.17f, 0.19f);
		Settings m_Settings;
		//entt::registry m_Registry;
	};
} // namespace RMC