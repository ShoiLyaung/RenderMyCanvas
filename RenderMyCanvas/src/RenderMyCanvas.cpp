#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer/Renderer.h"
#include "DrawBoard/DrawBoard.h"

using namespace Walnut;

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
		if (m_CurrentRenderer == &m_DrawBoard)
		{
			if (ImGui::CollapsingHeader("Shapes"))
			{
				if (ImGui::Button("Line"))
				{
					m_DrawBoard.AddPrimitive(PrimitiveFactory::CreateLine({ 0, 0 }, { 100, 100 }));
				}
				if (ImGui::Button("Circle"))
				{
					m_DrawBoard.AddPrimitive(PrimitiveFactory::CreateCircle({ 50, 50 }, 25.0f));
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

		auto image = m_CurrentRenderer->GetFinalImage();
		if (image)
			ImGui::Image(
				image->GetDescriptorSet(),
				{ (float)image->GetWidth(), (float)image->GetHeight() },
				ImVec2(0, 1),
				ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_CurrentRenderer->OnResize(m_ViewportWidth, m_ViewportHeight);
		m_CurrentRenderer->Render();

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	enum class Tool
	{
		None,
		Fill,
		Clip,
		Transform
	};
	Tool m_CurrentTool = Tool::None;
	Renderer m_Renderer;
	DrawBoard m_DrawBoard;
	Renderer* m_CurrentRenderer = &m_Renderer;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "RenderMyCanvas";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<MainLayer>();
	app->SetMenubarCallback([app]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				ImGui::EndMenu();
			}
		});
	return app;
}