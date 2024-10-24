#pragma once

#include <entt.hpp>

class InputSystem
{
public:
    static void Update(entt::registry& registry, float deltaTime);
};