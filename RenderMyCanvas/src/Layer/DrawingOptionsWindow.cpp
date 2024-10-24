// DrawingOptionsWindow.cpp
#include "DrawingOptionsWindow.h"

namespace RMC {
    void DrawingOptionsWindow::Render(float& lastRenderTime, ToolManager::Tool& currentTool, DrawBoard& drawBoard, Scene& scene, Camera& camera)
    {
        // 获取entt::registry
        auto& registry = scene.GetRegistry();

        // 开始ImGui窗口
        ImGui::Begin("Scene");
        ImGui::Text("Render Time: %.3fms", lastRenderTime);
        ImGui::Checkbox("Night", &scene.GetSettings().night);

        // 迭代具有SphereComponent和TransformComponent的实体
        {
            ImGui::Text("Spheres:");
            auto view = registry.view<SphereComponent, TransformComponent>();

            int i = 0;
            for (auto entity : view)
            {
                ImGui::PushID(i++);

                auto& sphere = view.get<SphereComponent>(entity);
                auto& transform = view.get<TransformComponent>(entity);

                ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);
                ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);

                // 如果实体具有MaterialComponent，显示材质属性
                if (registry.any_of<MaterialComponent>(entity))
                {
                    auto& material = registry.get<MaterialComponent>(entity);
                    ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
                    ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
                    ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
                    ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.EmissionColor));
                    ImGui::DragFloat("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);
                }

                ImGui::Separator();

                ImGui::PopID();
            }
        }

        ImGui::End();

        // 绘图选项窗口（保持不变）
        ImGui::Begin("Drawing Options");
        ImGui::Text("Render Time: %.3fms", lastRenderTime);
        if (ImGui::CollapsingHeader("Shapes"))
        {
            if (ImGui::Button("None"))
            {
                currentTool = ToolManager::Tool::None;
                drawBoard.SetDrawingMode(DrawingMode::None);
            }
            if (ImGui::Button("Line"))
            {
                currentTool = ToolManager::Tool::Line;
                drawBoard.SetDrawingMode(DrawingMode::Line);
            }
            if (ImGui::CollapsingHeader("Circle"))
            {
                if (ImGui::Button("Center-Radius"))
                {
                    currentTool = ToolManager::Tool::Circle;
                    drawBoard.SetDrawingMode(DrawingMode::Circle_CenterRadius);
                }
                if (ImGui::Button("Diameter"))
                {
                    currentTool = ToolManager::Tool::Circle;
                    drawBoard.SetDrawingMode(DrawingMode::Circle_Diameter);
                }
            }
            if (ImGui::CollapsingHeader("Ellipse"))
            {
                if (ImGui::Button("Foci"))
                {
                    currentTool = ToolManager::Tool::Ellipse;
                    drawBoard.SetDrawingMode(DrawingMode::Ellipse_Foci);
                }
                if (ImGui::Button("Center-Axes"))
                {
                    currentTool = ToolManager::Tool::Ellipse;
                    drawBoard.SetDrawingMode(DrawingMode::Ellipse_CenterAxes);
                }
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
    }
}