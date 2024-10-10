#include "SettingWindow.h"

namespace RMC::layer {
    void SettingWindow::Render(float& lastRenderTime, Renderer*& currentRenderer, DrawBoard& drawBoard, Renderer& renderer)
    {
        ImGui::Begin("Setting");
        ImGui::Text("Render Time: %.3fms", lastRenderTime);
        if (ImGui::Button("Dual Vector Foil"))
        {
            currentRenderer = &drawBoard;
        }
        if (ImGui::Button("3D Expansion"))
        {
            currentRenderer = &renderer;
        }
        ImGui::End();
    }
}