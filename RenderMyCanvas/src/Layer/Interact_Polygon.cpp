#include "pch.hpp"

#include "Layer.hpp"
#include "Primitive/LineSegment.hpp"
#include "Primitive/Polygon.hpp"

#include "Component/Artist.hpp"
#include "Component/VertexList.hpp"
#include "Component/DrawAlgo2D.hpp"
#include "Component/OptionMap.hpp"

namespace RMC::layer {
    void Layer::Interact_Polygon()
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

        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Enter)) {
            m_settings.drawStep = 3;
        }

        static component::VertexList buffVertices;

        switch (m_settings.drawStep) {
        case 0: {
            m_scene.tempEntities.clear();
            auto polygon = CreatePrimitive<primitive::Polygon>();
            m_scene.entities.emplace_back(polygon);
            m_settings.drawStep = 1;
            break;
        }
        case 1: {
            if (!m_scene.tempEntities.empty() && !Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Right)) {
                // Get auxline
                auto& auxline = m_scene.tempEntities.back();
                // Get vlist_auxline
                auto vlist_auxline = auxline->GetComponent<component::VertexList>();
                // Set end point
                vlist_auxline->at(1).position = m_camera.ScreenToWorld(screenMouseCoord); vlist_auxline->at(1).material = m_globalMaterial_1;
            }

            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                auto worldMouseCoord = m_camera.ScreenToWorld(screenMouseCoord);
                // Create an vertex
                component::Vertex vertex{};
                vertex.position = worldMouseCoord;
                // Add vetex to local buffVertices
                buffVertices.emplace_back(vertex);

                // Create an auxline
                auto auxline = CreatePrimitive<primitive::LineSegment>();
                // Get vlist_auxline
                auto vlist_auxline = auxline->GetComponent<component::VertexList>();
                // Set start point and end point
                vlist_auxline->at(0).position = worldMouseCoord; vlist_auxline->at(0).material = m_globalMaterial_2;
                vlist_auxline->at(1).position = worldMouseCoord;
                auto algo_auxline = auxline->GetComponent<component::DrawAlgo2D>();
                algo_auxline->SetParams({ {"mode", uint8_t(2)} });
                // Add auxline to tempEntities
                m_scene.tempEntities.emplace_back(auxline);
                m_settings.drawStep = 2;
            }
            break;
        }
        case 2: {
            if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                m_settings.drawStep = 1;
            }
            break;
        }
        case 3: {
            if (!Walnut::Input::IsKeyDown(Walnut::KeyCode::Enter)) {
                glm::vec3 centroid = buffVertices.GetCentroid();
                for (auto& vertex : buffVertices.vertices) {
                    vertex.position -= centroid;
                }
                buffVertices.ReCalculateNormal();

                auto& polygon = m_scene.entities.back();
                auto vlist_polygon = polygon->GetComponent<component::VertexList>();
                *vlist_polygon = buffVertices;

                auto trsf_polygon = polygon->GetComponent<component::Transform>();
                trsf_polygon->translate = centroid;

                buffVertices.clear();
                m_scene.tempEntities.clear();
                m_settings.drawStep = 0;
                m_settings.curDrawing = Settings::CUR_DRAWING::NONE_SHAPE;
            }
            break;
        }
        }
    }
}