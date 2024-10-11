#pragma once

#include <memory>
#include <glm/glm.hpp>
//#include "Shader.h"

namespace RMC {
    struct RenderComponent {
        //std::shared_ptr<Shader> shader;
        glm::vec4 Color;
        bool castShadow;
    };
}