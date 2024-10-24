#pragma once
#include "Walnut/Application.h"
#include <glm/gtc/type_ptr.hpp>

#include "ToolManager.h"
#include "Renderer/Renderer.h"
#include "DrawBoard/DrawBoard.h"
#include <entt.hpp>

#include "Components/TransformComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MaterialComponent.h"

namespace RMC {
    class DrawingOptionsWindow
    {
    public:
        static void Render(float& lastRenderTime, ToolManager::Tool& currentTool, DrawBoard& drawBoard, Scene& scene, Camera& camera);
    };
}