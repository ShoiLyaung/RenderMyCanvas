#pragma once

#include <glm/glm.hpp>
#include "Primitives/Primitive.h"

namespace RMC {
	struct LineComponent : public Primitive2D {
	private:
		glm::vec3 Start;
		glm::vec3 End;
		float LineWidth;
	};
} // namespace RMC