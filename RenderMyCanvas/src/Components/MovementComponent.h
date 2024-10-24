#pragma once

#include <glm/glm.hpp>

struct MovementComponent
{
    glm::vec3 Velocity{ 0.0f, 0.0f, 0.0f };
    glm::vec3 Acceleration{ 0.0f, 0.0f, 0.0f };
    float Speed = 5.0f; // ÒÆ¶¯ËÙ¶È
};