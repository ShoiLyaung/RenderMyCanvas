#pragma once

#include <glm/glm.hpp>
#include "Primitives/Primitive.h"

namespace RMC {
	struct SphereComponent : public Primitive3D {
		glm::vec3 center;
		float radius;
	};
} // namespace RMC