#pragma once

#include <entt.hpp>
#include "Renderer/Renderer.h"
#include "Camera/Camera.h"

class RenderingSystem
{
public:
    static void Render(entt::registry& registry, RMC::Renderer& renderer, const RMC::Camera& camera);
};