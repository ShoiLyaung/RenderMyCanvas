#pragma once

#include <entt.hpp>

class MovementSystem
{
public:
    static void Update(entt::registry& registry, float deltaTime);
};