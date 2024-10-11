#pragma once
#include "Walnut/Application.h"
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Renderer.h"
#include "DrawBoard/DrawBoard.h"

namespace RMC {
    class SettingWindow
    {
    public:
        static void Render(float& lastRenderTime, Renderer*& currentRenderer, DrawBoard& drawBoard, Renderer& renderer, Scene& scene);
    };
}