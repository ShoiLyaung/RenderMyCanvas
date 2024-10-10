#pragma once
#include "Walnut/Application.h"

#include "Renderer/Renderer.h"
#include "DrawBoard/DrawBoard.h"

namespace RMC::layer {
    class SettingWindow
    {
    public:
        static void Render(float& lastRenderTime, Renderer*& currentRenderer, DrawBoard& drawBoard, Renderer& renderer);
    };
}
