#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Primitives/Primitive.h"

namespace RMC {
	struct CurveComponent : public Primitive2D {
		std::vector<glm::vec3> ControlPoints;
		enum class CurveType { Bezier, BSpline } type;
	};
} // namespace RMC