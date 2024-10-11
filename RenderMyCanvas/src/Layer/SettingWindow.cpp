#include "SettingWindow.h"

namespace RMC {
    void SettingWindow::Render(float& lastRenderTime, Renderer*& currentRenderer, DrawBoard& drawBoard, Renderer& renderer, Scene& scene)
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

        ImGui::Begin("Scene");
        for (size_t i = 0; i < scene.Spheres.size(); i++)
        {
            ImGui::PushID((int)i);
            Sphere& sphere = scene.Spheres[i];
            ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
            ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
            ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.Albedo));
            ImGui::Separator();
            ImGui::PopID();
        }
        ImGui::End();
    }
}