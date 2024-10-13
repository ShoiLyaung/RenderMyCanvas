#pragma once

#include <glm/glm.hpp>
#include "EllipseComponent.h"

namespace RMC {
    struct CircleComponent : public EllipseComponent {
	private:
        float Radius;
    };
} // namespace RMC