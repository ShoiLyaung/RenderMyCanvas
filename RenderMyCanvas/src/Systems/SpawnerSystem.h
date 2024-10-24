#pragma once

#include <entt.hpp>

class SpawnerSystem
{
public:
    static void Update(entt::registry& registry, float deltaTime);
};