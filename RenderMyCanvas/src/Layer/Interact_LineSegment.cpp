#include "pch.hpp"

#include "Layer.hpp"
#include "Primitive/LineSegment.hpp"

#include "Component/Artist.hpp"
#include "Component/VertexList.hpp"
#include "Component/DrawAlgo2D.hpp"
#include "Component/OptionMap.hpp"
#include "Component/Mode.hpp"

namespace RMC::layer {
    void Layer::Interact_LineSegment()
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

        switch (m_settings.drawStep) {
        case 0: {
            m_scene.tempEntities.clear();
            m_settings.drawStep = 1;
            break;
        }
        case 1: {
            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                auto worldMouseCoord = m_camera.ScreenToWorld(screenMouseCoord);
                // Create a line segment primitive
                auto lineSegment = CreatePrimitive<primitive::LineSegment>();
                // Get vertex list
                auto vlist = lineSegment->GetComponent<component::VertexList>();
                // Set start point
                vlist->at(0).position = worldMouseCoord;
                // Set end point
                vlist->at(1).position = worldMouseCoord;
                // Get artist
                auto artist = lineSegment->GetComponent<component::Artist>();
                artist->thickness = 3;
                // Add line segment to scene
                m_scene.entities.emplace_back(lineSegment);
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
            // Get line segment in scene
            auto& lineSegment = m_scene.entities.back();
            // Get vertex list
            auto vlist = lineSegment->GetComponent<component::VertexList>();
            // Set end point
            vlist->at(1).position = worldMouseCoord;
            // Set material
            vlist->at(0).material = m_globalMaterial_1;

            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                m_settings.drawStep = 4;
            }
            break;
        }
        case 4: {
            if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                m_settings.drawStep = 5;
            }
            break;
        }
        case 5: {
            // Get line segment in scene
            auto& lineSegment = m_scene.entities.back();
            // Get vertex list
            auto vlist = lineSegment->GetComponent<component::VertexList>();
            // Get centriod
            auto centriod = vlist->GetCentroid();
            // Translate start and end point to centriod
            vlist->at(0).position -= centriod;
            vlist->at(1).position -= centriod;
            // Set transform
            lineSegment->GetComponent<component::Transform>()->translate = centriod;

            m_scene.tempEntities.clear();
            m_settings.drawStep = 0;
            m_settings.curDrawing = Settings::CUR_DRAWING::NONE_SHAPE;
            break;
        }
        default: {
            break;
        }
        }
    }
}