#pragma once
#include "Walnut/Application.h"

#include "Renderer/Renderer.h"

namespace RMC {
    class SettingWindow
    {
    public:
        static void Render(float& lastRenderTime, Renderer*& currentRenderer, Renderer& renderer, Scene& scene, Camera& camera);
    };
} // namespace RMC