#include "pch.hpp"
#include "Layer.hpp"

#include "Primitive/LineSegment.hpp"
#include "Primitive/Arc.hpp"

namespace RMC::layer {
    void Layer::Interact_Arc()
    {
        static uint8_t s_drawStep = 0;
        glm::vec2 screenMouseCoord = this->GetViewportMouseCoord();
        glm::vec3 worldMouseCoord = m_camera.ScreenToWorld(screenMouseCoord);

        if (screenMouseCoord.x < 0 || screenMouseCoord.y < 0) return;
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Escape)) {
            m_scene.tempEntities.clear();
            if (m_settings.drawStep != 0) { // Has been initialized
                m_scene.entities.pop_back();
            }
            s_drawStep = 0;
            m_settings.curDrawing = Settings::CUR_DRAWING::NONE_SHAPE;
        }

        switch (s_drawStep)
        {
        case 0: {
            m_scene.tempEntities.clear();
            s_drawStep = 1;
            break;
        }
        case 1: {
            // If mouse left-button down, set translate of arc
            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                // Create arc primitive
                auto arc = this->CreatePrimitive<primitive::Arc>();
                auto trsf_arc = arc->GetComponent<component::Transform>();
                trsf_arc->translate = worldMouseCoord;
                m_scene.entities.emplace_back(arc);

                // Create aux line segment primitive
                auto auxLine = this->CreatePrimitive<primitive::LineSegment>();
                auto vlist_auxLine = auxLine->GetComponent<component::VertexList>();
                vlist_auxLine->at(0).position = vlist_auxLine->at(1).position = worldMouseCoord;
                auto algo_auxLine = auxLine->GetComponent<component::DrawAlgo2D>();
                algo_auxLine->SetParams({ {"mode", uint8_t(2)} });  // Dot line
                m_scene.tempEntities.emplace_back(auxLine);

                s_drawStep = 2;
            }
            break;
        }
        case 2: {
            if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                s_drawStep = 3;
            }
            break;
        }
        case 3: {
            // Trace aux line
            auto& auxline = m_scene.tempEntities.at(0);
            auto vlist_auxline = auxline->GetComponent<component::VertexList>();
            vlist_auxline->at(1).position = worldMouseCoord;
            auto artist = auxline->GetComponent<component::Artist>();
            artist->color = utils::RGBA_to_uint32_t(m_globalMaterial_2.Albedo);

            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                const glm::mat4& cameraView = m_camera.GetView();
                glm::vec3 rightDir = glm::vec3(cameraView[0]);
                auto& arc = m_scene.entities.back();
                auto trsf_arc = arc->GetComponent<component::Transform>();
                auto vlist_arc = arc->GetComponent<component::VertexList>();
                float radius = glm::distance(trsf_arc->translate, worldMouseCoord);

                ////glm::mat4 rotation1 = glm::mat4_cast(utils::GetRotationQuat(m_camera.GetForwardDirection(), { 0,0,-1 }));
                glm::mat4 rotation2 = glm::mat4_cast(utils::GetRotationQuat(vlist_auxline->at(1).position - vlist_auxline->at(0).position, { 1, 0, 0 }));
                ////glm::mat4 rotationFin = rotation2 * rotation1;

                trsf_arc->rotation = glm::toQuat(rotation2);

                vlist_arc->at(0).position = trsf_arc->InverseTransformPoint(worldMouseCoord);
                s_drawStep = 4;
            }
            break;
        }
        case 4: {
            if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                s_drawStep = 5;
            }
            break;
        }
        case 5: {
            // Trace arc
            auto& arc = m_scene.entities.back();
            auto trsf_arc = arc->GetComponent<component::Transform>();
            auto vlist_arc = arc->GetComponent<component::VertexList>();
            vlist_arc->at(1).position = trsf_arc->InverseTransformPoint(worldMouseCoord);

            auto artist = arc->GetComponent<component::Artist>();
            artist->color = utils::RGBA_to_uint32_t(m_globalMaterial_1.Albedo);
            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                s_drawStep = 6;
            }
            break;
        }
        case 6: {
            m_scene.tempEntities.clear();
            s_drawStep = 0;
            m_settings.curDrawing = Settings::CUR_DRAWING::NONE_SHAPE;
            break;
        }
        default: {
            throw std::runtime_error(std::format("Invalid case {}.", s_drawStep));
            break;
        }
        }
    }
}