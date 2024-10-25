#include "SettingWindow.h"

namespace RMC {
    void SettingWindow::Render(float& lastRenderTime, Renderer*& currentRenderer, DrawBoard& drawBoard, Renderer& renderer, Scene& scene, Camera& camera)
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
        ImGui::Checkbox("DLSS", &renderer.DLSSEnabled);
        ImGui::Checkbox("Accumulate", &renderer.GetSettings().Accumulate);
        if (ImGui::Button("output"))
        {
            renderer.outputImage();
        }
        ImGui::Text("origin: %d, downscale: %d", renderer.original_image_cnt, renderer.downsample_image_cnt);
        if (ImGui::Button("Reset"))
            renderer.ResetFrameIndex();
        ImGui::End();
    }
} // namespace RMC