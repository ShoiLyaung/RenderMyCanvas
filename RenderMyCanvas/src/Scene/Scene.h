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
		std::vector<Material> Materials;
		glm::vec3 GetSkyColor() const { return skyColor; }
		Settings& GetSettings() { return m_Settings; }
		bool IsNight() const { return m_Settings.night; }
		//entt::entity CreateEntity();

		bool OnUpdate(float ts);
	protected:
		glm::vec3 skyColor = glm::vec3(0.6f, 0.7f, 0.9f);
		Settings m_Settings;
		//entt::registry m_Registry;
	};
} // namespace RMC