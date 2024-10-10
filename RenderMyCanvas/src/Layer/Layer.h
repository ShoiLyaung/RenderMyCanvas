#pragma once
#include "Walnut/Application.h"
#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer/Renderer.h"
#include "DrawBoard/DrawBoard.h"

namespace RMC::layer {
	class MainLayer : public Walnut::Layer
	{
	public:
		virtual void OnUIRender() override
		{
			// Setting
			ImGui::Begin("Setting");
			ImGui::Text("Render Time: %.3fms", m_LastRenderTime);
			if (ImGui::Button("Dual Vector Foil"))
			{
				m_CurrentRenderer = &m_DrawBoard;
			}
			if (ImGui::Button("3D Expansion"))
			{
				m_CurrentRenderer = &m_Renderer;
			}
			ImGui::End();

			//Drawing Options
			ImGui::Begin("Drawing Options");
			ImGui::Text("Render Time: %.3fms", m_LastRenderTime);
			if (m_CurrentRenderer == &m_DrawBoard)
			{
				if (ImGui::CollapsingHeader("Shapes"))
				{
					if (ImGui::Button("Line"))
					{
						m_CurrentTool = Tool::Line;
					}
					if (ImGui::Button("Circle"))
					{
						m_CurrentTool = Tool::Circle;
					}
					// Add more shape options here (e.g., Arc, Ellipse, Triangle, etc.)
				}

				if (ImGui::CollapsingHeader("Tools"))
				{
					if (ImGui::Button("Fill"))
					{
						m_CurrentTool = Tool::Fill;
					}
					if (ImGui::Button("Clip"))
					{
						m_CurrentTool = Tool::Clip;
					}
					if (ImGui::Button("Transform"))
					{
						m_CurrentTool = Tool::Transform;
					}
					// Add more tool options here
				}
			}
			ImGui::End();

			// Viewport
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Viewport");

			m_ViewportWidth = (uint32_t)ImGui::GetContentRegionAvail().x;
			m_ViewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y;

			ImVec2 viewportPos = ImGui::GetWindowPos();
			ImVec2 cursorPos = ImGui::GetCursorScreenPos();

			auto image = m_CurrentRenderer->GetFinalImage();
			if (image)
				ImGui::Image(
					image->GetDescriptorSet(),
					{ (float)image->GetWidth(), (float)image->GetHeight() }
					//ImVec2(0, 1),
					//ImVec2(1, 0)
				);

			ImGui::End();
			ImGui::PopStyleVar();

			m_ViewportOffset = { cursorPos.x, cursorPos.y };

			Render();
		}

		virtual void OnUpdate(float ts) override
		{
			// Handle mouse input for drawing in DrawBoard
			if (m_CurrentRenderer == &m_DrawBoard)
			{
				ImGuiIO& io = ImGui::GetIO();
				glm::vec2 mousePos = glm::vec2(io.MousePos.x - m_ViewportOffset.x, io.MousePos.y - m_ViewportOffset.y);
				// Limit drawing to inside the viewport bounds
				if (m_CurrentTool != Tool::None && mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x < m_ViewportWidth && mousePos.y < m_ViewportHeight)
				{
					// if (io.MouseClicked[0]) // Left mouse button is clicked
					// {
					// 	m_DrawBoard.OnMouseEvent(0, 1, (int)mousePos.x, (int)mousePos.y);
					// }
					// else
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					if (io.MouseDown[0]) // Left mouse button is down
					{
						if (!m_IsDrawing)
						{
							m_DrawStart = mousePos;
							m_IsDrawing = true;
						}
						else
						{
							m_DrawBoard.SetTemporaryPrimitive(PrimitiveFactory::CreateLine(m_DrawStart, mousePos));
						}
					}
					else if (m_IsDrawing)
					{
						// Finalize the drawing
						m_DrawBoard.AddPrimitive(PrimitiveFactory::CreateLine(m_DrawStart, mousePos));
						m_DrawBoard.ClearTemporaryPrimitive();
						m_IsDrawing = false;
					}
				}
			}
		}

		void Render()
		{
			Walnut::Timer timer;

			m_CurrentRenderer->OnResize(m_ViewportWidth, m_ViewportHeight);
			m_CurrentRenderer->Render();

			m_LastRenderTime = timer.ElapsedMillis();
		}
	private:
		enum class Tool
		{
			None,
			Line,
			Circle,
			Fill,
			Clip,
			Transform
		};
		Tool m_CurrentTool = Tool::None;
		Renderer m_Renderer;
		DrawBoard m_DrawBoard;
		Renderer* m_CurrentRenderer = &m_DrawBoard;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		float m_LastRenderTime = 0.0f;
		bool m_IsDrawing = false;
		glm::vec2 m_DrawStart;
		glm::vec2 m_ViewportOffset;
	};
} // namespace RMC::layer