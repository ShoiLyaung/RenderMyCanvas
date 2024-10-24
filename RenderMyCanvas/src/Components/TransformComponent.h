#pragma once

#include <glm/glm.hpp>

struct TransformComponent
{
    glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
    glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };
};