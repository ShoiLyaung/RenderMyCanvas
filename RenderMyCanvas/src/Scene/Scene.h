#pragma once

#include <vector>
//#include "entt.hpp"

#include "Primitives/Sphere.h"
#include "Material/Material.h"

namespace RMC
{
	class Scene
	{
	public:
		Scene() ;
		~Scene();
		//std::vector<Primitive> Primitives;
		std::vector<Sphere> Spheres;
		std::vector<Material> Materials;

		//entt::entity CreateEntity();

		bool OnUpdate(float ts);
	private:
		//entt::registry m_Registry;
	};
} // namespace RMC