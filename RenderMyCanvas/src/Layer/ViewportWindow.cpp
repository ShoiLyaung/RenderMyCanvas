#include "ViewportWindow.h"

namespace RMC::layer {
   void ViewportWindow::Render(uint32_t& viewportWidth, uint32_t& viewportHeight, glm::vec2& viewportOffset, Renderer* currentRenderer, float& lastRenderTime, Camera& camera)
   {
       ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
       ImGui::Begin("Viewport");

       viewportWidth = (uint32_t)ImGui::GetContentRegionAvail().x;
       viewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y;

       ImVec2 viewportPos = ImGui::GetWindowPos();
       ImVec2 cursorPos = ImGui::GetCursorScreenPos();

       auto image = currentRenderer->GetFinalImage();
       if (image)
           ImGui::Image(
               image->GetDescriptorSet(),
               { (float)image->GetWidth(), (float)image->GetHeight() }
           );

       ImGui::End();
       ImGui::PopStyleVar();

       viewportOffset = { cursorPos.x, cursorPos.y };

       Walnut::Timer timer;
       camera.OnResize(viewportWidth, viewportHeight);
       currentRenderer->OnResize(viewportWidth, viewportHeight);
       currentRenderer->Render(camera); // Pass the dereferenced camera pointer
       lastRenderTime = timer.ElapsedMillis();
   }
}