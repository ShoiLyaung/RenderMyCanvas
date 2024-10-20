#include "pch.hpp"

#include "Layer.hpp"
#include "Primitive/LineSegment.hpp"
#include "Primitive/Curve.hpp"

#include "Component/Artist.hpp"
#include "Component/VertexList.hpp"
#include "Component/DrawAlgo2D.hpp"
#include "Component/OptionMap.hpp"

namespace RMC::layer {
    void Layer::Interact_Curve()
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

        switch (m_settings.drawStep)
        {
        case 0: {
            m_scene.tempEntities.clear();
            auto curve = CreatePrimitive<primitive::Curve>();
            auto artist = curve->GetComponent<component::Artist>();
            artist->thickness = 5;
            m_scene.entities.emplace_back(curve);
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
                vlist_auxline->at(1).position = m_camera.ScreenToWorld(screenMouseCoord);
            }

            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                auto worldMouseCoord = m_camera.ScreenToWorld(screenMouseCoord);
                // Get vlist
                auto vlist_curve = m_scene.entities.back()->GetComponent<component::VertexList>();
                // Push back vertex
                component::Vertex vertex{};
                vertex.position = worldMouseCoord;
                vlist_curve->emplace_back(worldMouseCoord);
                vlist_curve->at(0).material = m_globalMaterial_1;

                // Create an auxline
                auto auxline = CreatePrimitive<primitive::LineSegment>();
                // Get vlist_auxline
                auto vlist_auxline = auxline->GetComponent<component::VertexList>();
                // Set start point and end point
                vlist_auxline->at(0).position = worldMouseCoord; vlist_auxline->at(0).material = m_globalMaterial_2;
                vlist_auxline->at(1).position = worldMouseCoord;
                // Create option map
                component::OptionMap optMap{};
                optMap.set("line_shape", "dash");
                // Add option map to auxline
                auxline->AddComponent<component::OptionMap>(optMap);
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
                // Get vlist
                auto vlist_curve = m_scene.entities.back()->GetComponent<component::VertexList>();
                // Get centroid
                auto centroid = vlist_curve->GetCentroid();
                // Translate all vertices to centriod
                for (auto& vertex : vlist_curve->vertices) {
                    vertex.position -= centroid;
                }
                // Get transform
                auto trsf = m_scene.entities.back()->GetComponent<component::Transform>();
                // Set translate of trsf
                trsf->translate = centroid;

                m_scene.tempEntities.clear();
                m_settings.drawStep = 0;
                m_settings.curDrawing = Settings::CUR_DRAWING::NONE_SHAPE;
            }
            break;
        }
        default: break;
        }
    }
}