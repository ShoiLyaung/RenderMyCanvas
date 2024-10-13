#pragma once

#include <glm/glm.hpp>
#include "Primitives/Primitive.h"

namespace RMC {
    class EllipseComponent : public Primitive2D {
	private:
		glm::vec3 F1;
		glm::vec3 F2;
        float MajorAxis;
        float MinorAxis;
        float StartAngle;
        float EndAngle;
    };
} // namespace RMC