#include "pch.hpp"

#include "Layer.hpp"
#include "Primitive/Ellipse.hpp"

#include "Component/Artist.hpp"
#include "Component/VertexList.hpp"
#include "Component/DrawAlgo2D.hpp"
#include "Component/OptionMap.hpp"

namespace RMC::layer {
    void Layer::Interact_Ellipse()
    {
        glm::vec2 screenMouseCoord = this->GetViewportMouseCoord();
        if (screenMouseCoord.x < 0 || screenMouseCoord.y < 0) return;
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Escape)) {
            m_scene.tempEntities.clear();
            if (m_settings.drawStep != 0) { // Has been initialized
                m_scene.entities.pop_back();
            }
            m_settings.drawStep = 0;
            m_settings.curDrawing = Settings::CUR_DRAWING::NONE_SHAPE;
        }

        const auto& cameraView = m_camera.GetView();

        static glm::vec3 s_worldEllipseCenter{};
        static glm::vec2 s_screenEllipseCenter{};

        switch (m_settings.drawStep)
        {
        case 0: {
            m_scene.tempEntities.clear();
            m_settings.drawStep = 1;
            break;
        }
        case 1: {
            // If mouse left-button down, set the center of ellipse
            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                auto worldMouseCoord = m_camera.ScreenToWorld(screenMouseCoord);
                // Create an ellipse primitive
                auto ellipse = CreatePrimitive<primitive::Ellipse>();
                // Record center in `s_worldEllipseCenter` and `s_screenEllipseCenter`
                s_worldEllipseCenter = worldMouseCoord;
                s_screenEllipseCenter = m_camera.WorldToScreen(worldMouseCoord);
                // Get ellipse transform
                auto trsf_ellipse = ellipse->GetComponent<component::Transform>();
                // Set translate
                trsf_ellipse->translate = s_worldEllipseCenter;
                // Add to scene
                m_scene.entities.emplace_back(ellipse);
                // Go to case 2
                m_settings.drawStep = 2;
            }
            break;
        }
        case 2: {
            if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                m_settings.drawStep = 3;
            }
            break;
        }
        case 3: {
            auto worldMouseCoord = m_camera.ScreenToWorld(screenMouseCoord);
            // Get the ellipse
            auto& ellipse = m_scene.entities.back();
            // Get vertex list
            auto vlist = ellipse->GetComponent<component::VertexList>();
            // Get camera view mat
            const glm::mat4& cameraView = m_camera.GetView();
            // Calculate right direction and up direction
            glm::vec3 rightDir = glm::vec3(cameraView[0]), upDir = glm::vec3(cameraView[1]);
            // Calculate x-radius
            float x_radius = glm::dot(worldMouseCoord - s_worldEllipseCenter, rightDir);
            // Calculate y-radius
            float y_radius = glm::dot(worldMouseCoord - s_worldEllipseCenter, upDir);
            // Set x-apex
            vlist->at(0).position = { x_radius,0.0f,0.0f };
            // Set y-apex
            vlist->at(1).position = { 0.0f, y_radius,0.0f };
            // Set neg x-apex
            vlist->at(2).position = { -x_radius,0.0f,0.0f };
            // Set neg y-apex
            vlist->at(3).position = { 0.0f, -y_radius,0.0f };
            // Get transform
            auto trsf_ellipse = ellipse->GetComponent<component::Transform>();
            // Set rotation of transform
            trsf_ellipse->rotation = utils::GetRotationQuat(m_camera.GetForwardDirection(), { 0.0f,0.0f,-1.0f });

            // If mouse left-button down, go to case 4
            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                m_settings.drawStep = 4;
            }
            break;
        }
        case 4: {
            if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                m_scene.tempEntities.clear();
                m_settings.drawStep = 0;
                m_settings.curDrawing = Settings::CUR_DRAWING::NONE_SHAPE;
                m_settings.drawStep = 0;
            }
            break;
        }
        default: {
            throw std::runtime_error(std::format("Invalid case {}.", m_settings.drawStep));
            break;
        }
        }
    }
}