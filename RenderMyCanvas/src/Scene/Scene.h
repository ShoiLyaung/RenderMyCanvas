#pragma once

#include <vector>
#include "entt.hpp"

#include "Primitives/Sphere.h"

namespace RMC
{
	class Scene
	{
	public:
		//std::vector<Primitive> Primitives;
		std::vector<Sphere> Spheres;
	};
} // namespace RMC