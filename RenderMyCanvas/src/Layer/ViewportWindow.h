#pragma once
#include "Walnut/Application.h"
#include "Walnut/Timer.h"

#include "Renderer/Renderer.h"
#include "DrawBoard/DrawBoard.h"
#include "Camera/Camera.h"

namespace RMC {
    class ViewportWindow
    {
    public:
		static void Render(uint32_t& viewportWidth, uint32_t& viewportHeight, glm::vec2& viewportOffset, Renderer* currentRenderer, float& lastRenderTime, Scene& scene, Camera& camera);
    };
}