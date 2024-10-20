#pragma once
#include <glm/glm.hpp>

namespace RMC
{
	struct Sphere
	{
		glm::vec3 Position{ 0.0f };
		float Radius = 0.5f;
		
		int MaterialIndex = 0;
	};
} // namespace RMC