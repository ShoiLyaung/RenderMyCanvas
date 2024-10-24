#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <entt.hpp>
#include <random>

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
		const entt::registry& GetRegistry() const { return m_Registry; }
		entt::registry& GetRegistry() { return m_Registry; }
		entt::entity CreateEntity();
		bool OnUpdate(float ts);

		glm::vec3 GetSkyColor() const { return skyColor; }
		Settings& GetSettings() { return m_Settings; }
		bool IsNight() const { return m_Settings.night; }
		float RandomFloat(float min, float max);

	protected:
		entt::registry m_Registry;
		glm::vec3 skyColor = glm::vec3(0.6f, 0.7f, 0.9f);
		Settings m_Settings;
	};
} // namespace RMC