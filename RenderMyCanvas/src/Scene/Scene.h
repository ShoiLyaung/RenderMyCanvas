#pragma once

#include <vector>
#include "entt.hpp"

#include "Primitives/Sphere.h"

namespace RMC
{
	class Scene
	{
	public:
		Scene() ;
		~Scene();
		//std::vector<Primitive> Primitives;
		std::vector<Sphere> Spheres;

		entt::entity CreateEntity();

		void OnUpdate(float ts);
	private:
		entt::registry m_Registry;
	};
} // namespace RMC