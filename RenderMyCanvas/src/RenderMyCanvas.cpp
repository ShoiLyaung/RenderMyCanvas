#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Layer/Layer.hpp"

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "RenderMyCanvas";

	Walnut::Application* app = new Walnut::Application(spec);
    std::shared_ptr<RMC::layer::Layer> mainLayer =
        std::make_shared<RMC::layer::Layer>();
	app->PushLayer(mainLayer);
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