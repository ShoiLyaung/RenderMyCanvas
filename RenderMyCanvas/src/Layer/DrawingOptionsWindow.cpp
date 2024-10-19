#include "DrawingOptionsWindow.h"
#include <iostream>

namespace RMC {
    void DrawingOptionsWindow::Render(float& lastRenderTime, ToolManager::Tool& currentTool, DrawBoard& drawBoard, Scene& scene, Camera& camera)
    {
        ImGui::Begin("Scene");
        ImGui::Text("Render Time: %.3fms", lastRenderTime);
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

        ImGui::Begin("Drawing Options");
        ImGui::Text("Render Time: %.3fms", lastRenderTime);
        int lineWidth_t = drawBoard.GetLineWidth()*10;
        ImGui::DragInt("Line Width", &lineWidth_t, 0.05f, 1, 100);
        drawBoard.SetLineWidth(lineWidth_t/10.0f);
        uint32_t lineColor_t = drawBoard.GetLineColor();
        uint32_t fillColor_t = drawBoard.GetFillColor();
        ImVec4 lineColor = ImGui::ColorConvertU32ToFloat4(lineColor_t);
        if (ImGui::ColorEdit4("Line Color", (float*)&lineColor)) {
            lineColor_t = ImGui::ColorConvertFloat4ToU32(lineColor);
            drawBoard.SetLineColor(lineColor_t);
        }
        ImVec4 fillColor = ImGui::ColorConvertU32ToFloat4(fillColor_t);
        if (ImGui::ColorEdit4("Fill Color", (float*) &fillColor))
		{
            fillColor_t = ImGui::ColorConvertFloat4ToU32(fillColor);
			drawBoard.SetFillColor(fillColor_t);
		}
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
                if(ImGui::Button("Foci-Bersenham"))
				{
					currentTool = ToolManager::Tool::Ellipse;
					drawBoard.SetDrawingMode(DrawingMode::Ellipse_Foci_Bersenham);
				}
                if (ImGui::Button("Center-Axes-Bersenham"))
                {
                    currentTool = ToolManager::Tool::Ellipse;
					drawBoard.SetDrawingMode(DrawingMode::Ellipse_CenterAxes_Bersenham);
				}
            }
            if (ImGui::CollapsingHeader("Arc")) {
                if (ImGui::Button("Circle Arc")) {
                    currentTool = ToolManager::Tool::Arc;
                    drawBoard.SetDrawingMode(DrawingMode::Arc_Circle);
                }
                if (ImGui::Button("Ellipse Arc")) {
                    currentTool = ToolManager::Tool::Arc;
                    drawBoard.SetDrawingMode(DrawingMode::Arc_Ellipse);
                }
            }
        }

        if (ImGui::CollapsingHeader("Tools"))
        {
            if (ImGui::Button("Fill"))
            {
                currentTool = ToolManager::Tool::Fill;
                drawBoard.SetDrawingMode(DrawingMode::Fill);
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