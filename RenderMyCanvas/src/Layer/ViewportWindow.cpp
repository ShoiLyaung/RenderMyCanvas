#include "ViewportWindow.h"
#include "Network/Network.h"
namespace RMC {
    void ViewportWindow::Render(uint32_t& viewportWidth, uint32_t& viewportHeight, glm::vec2& viewportOffset, Renderer* currentRenderer, float& lastRenderTime, Scene& scene, Camera& camera)
   {
       ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
       ImGui::Begin("Viewport");

       auto image = currentRenderer->GetFinalImage();
	   auto scale = currentRenderer->GetImageScale();

	   viewportWidth = (uint32_t)ImGui::GetContentRegionAvail().x / scale;
	   viewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y / scale;

       ImVec2 viewportPos = ImGui::GetWindowPos();
       ImVec2 cursorPos = ImGui::GetCursorScreenPos();

       
       if (currentRenderer -> has_img)
       {
           std::cout << "start" << image->GetWidth()<<' '<< image->GetHeight() << std::endl;
           uint32_t* final_image_data = currentRenderer->m_PpPipeline->process(currentRenderer->m_outImageData, image->GetWidth(), image->GetHeight());
           std::cout << "mid" << std::endl;
           Walnut::Image final_img(image->GetWidth(), image->GetHeight(), Walnut::ImageFormat::RGBA, final_image_data);
           std::cout << "end" << std::endl;
           ImGui::Image(
               final_img.GetDescriptorSet(),
			   { 
                   (float)image->GetWidth(), 
				   (float)image->GetHeight()
               },
               currentRenderer->Get_uv0(),
			   currentRenderer->Get_uv1()
           );
           delete[] final_image_data;
       }

       ImGui::End();
       ImGui::PopStyleVar();

       viewportOffset = { cursorPos.x, cursorPos.y };

       Walnut::Timer timer;
       currentRenderer->OnResize(viewportWidth, viewportHeight);
       camera.OnResize(viewportWidth, viewportHeight);
       currentRenderer->Render(scene, camera);
       lastRenderTime = timer.ElapsedMillis();
   }
}