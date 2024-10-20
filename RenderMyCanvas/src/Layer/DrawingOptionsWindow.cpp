#include "DrawingOptionsWindow.h"

namespace RMC {
    void DrawingOptionsWindow::Render(float& lastRenderTime, ToolManager::Tool& currentTool, DrawBoard& drawBoard, Scene& scene, Camera& camera)
    {
        ImGui::Begin("Scene");
        ImGui::Text("Render Time: %.3fms", lastRenderTime);
		ImGui::Checkbox("night", &scene.GetSettings().night);

        for (size_t i = 0; i < scene.Spheres.size(); i++)
        {
            ImGui::PushID((int)i);

            Sphere& sphere = scene.Spheres[i];
            ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
            ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
            ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, (int)scene.Materials.size() - 1);

            ImGui::Separator();

            ImGui::PopID();
        }

        for (size_t i = 0; i < scene.Materials.size(); i++)
        {
            ImGui::PushID((int)i);

            Material& material = scene.Materials[i];
            ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
            ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
            ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
            ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.EmissionColor));
            ImGui::DragFloat("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);

            ImGui::Separator();

            ImGui::PopID();
        }
        ImGui::End();

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