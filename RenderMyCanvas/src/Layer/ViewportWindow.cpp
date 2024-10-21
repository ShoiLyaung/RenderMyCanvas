#include "ViewportWindow.h"

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

       if (image)
           ImGui::Image(
               image->GetDescriptorSet(),
			   { 
                   (float)image->GetWidth() * scale, 
				   (float)image->GetHeight()* scale
               },
               currentRenderer->Get_uv0(),
			   currentRenderer->Get_uv1()
           );

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