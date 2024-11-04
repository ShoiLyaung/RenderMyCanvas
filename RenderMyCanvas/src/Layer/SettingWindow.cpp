#include "SettingWindow.h"
#include "Network/Network.h"
namespace RMC {
    void SettingWindow::Render(float& lastRenderTime, Renderer*& currentRenderer, Renderer& renderer, Scene& scene, Camera& camera)
    {
        ImGui::Begin("Setting");
        ImGui::Text("Render Time: %.3fms", lastRenderTime);
        ImGui::Checkbox("Accumulate", &renderer.GetSettings().Accumulate);
        if (ImGui::Button("Reset"))
            renderer.ResetFrameIndex();
        ImGui::End();
    }
} // namespace RMC