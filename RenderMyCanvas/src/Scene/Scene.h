#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
//#include "entt.hpp"

#include "Game/BallGame.h"
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
		Scene();
		~Scene();
		std::vector<std::unique_ptr<Sphere>> Spheres;
		std::vector<Material> Materials;
		glm::vec3 GetSkyColor() const { return skyColor; }
		Settings& GetSettings() { return m_Settings; }
		bool IsNight() const { return m_Settings.night; }

		bool OnUpdate(float ts, BallGame& ballGame);
	protected:
		glm::vec3 skyColor = glm::vec3(0.16f, 0.17f, 0.19f);
		Settings m_Settings;
		//entt::registry m_Registry;
	};
} // namespace RMC