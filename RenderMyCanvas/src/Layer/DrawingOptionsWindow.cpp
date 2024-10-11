#include "DrawingOptionsWindow.h"

namespace RMC {
    void DrawingOptionsWindow::Render(float& lastRenderTime, ToolManager::Tool& currentTool, DrawBoard& drawBoard, Scene& scene, Camera& camera)
    {
        ImGui::Begin("Drawing Options");
        ImGui::Text("Render Time: %.3fms", lastRenderTime);
        if (ImGui::CollapsingHeader("Shapes"))
        {
            if (ImGui::Button("None"))
            {
                currentTool = ToolManager::Tool::None;
            }
            if (ImGui::Button("Line"))
            {
                currentTool = ToolManager::Tool::Line;
            }
            if (ImGui::Button("Circle"))
            {
                currentTool = ToolManager::Tool::Circle;
            }
        }

        if (ImGui::CollapsingHeader("Tools"))
        {
            if (ImGui::Button("Fill"))
            {
                currentTool = ToolManager::Tool::Fill;
            }
            if (ImGui::Button("Clip"))
            {
                currentTool = ToolManager::Tool::Clip;
            }
            if (ImGui::Button("Transform"))
            {
                currentTool = ToolManager::Tool::Transform;
            }
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