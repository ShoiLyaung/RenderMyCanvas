#include "ViewportWindow.h"

namespace RMC {
   void ViewportWindow::Render(uint32_t& viewportWidth, uint32_t& viewportHeight, glm::vec2& viewportOffset, Renderer* currentRenderer, float& lastRenderTime, Scene& scene, Camera& camera)
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
       currentRenderer->OnResize(viewportWidth, viewportHeight);
       camera.OnResize(viewportWidth, viewportHeight);
       currentRenderer->Render(scene, camera);
       lastRenderTime = timer.ElapsedMillis();
   }
}