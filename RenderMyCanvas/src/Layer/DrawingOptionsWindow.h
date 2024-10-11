#pragma once
#include "Walnut/Application.h"

#include "ToolManager.h"
#include "Renderer/Renderer.h"
#include "DrawBoard/DrawBoard.h"


namespace RMC {
    class DrawingOptionsWindow
    {
    public:
        static void Render(float& lastRenderTime, ToolManager::Tool& currentTool, DrawBoard& drawBoard);
    };
}