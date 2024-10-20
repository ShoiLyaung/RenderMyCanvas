#include "pch.hpp"

#include "Layer.hpp"
#include "Primitive/Parent.hpp"

#include "Component/Artist.hpp"
#include "Component/VertexList.hpp"
#include "Component/DrawAlgo2D.hpp"
#include "Component/OptionMap.hpp"

namespace RMC::layer {
    void Layer::Interact_Parent()
    {
        glm::vec2 screenMouseCoord = this->GetViewportMouseCoord();
        if (screenMouseCoord.x < 0 || screenMouseCoord.y < 0) return;

        switch (m_settings.drawStep) {
        case 0: {
            m_scene.tempEntities.clear();
            m_settings.drawStep = 1;
            break;
        }
        case 1: {
            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                m_settings.drawStep = 2;
            }
            break;
        }
        case 2: {
            if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                auto parent = this->CreatePrimitive<primitive::Parent>();
                auto artist = parent->GetComponent<component::Artist>();
                artist->color = utils::RGBA_to_uint32_t(m_globalMaterial_1.Albedo);
                auto vlist = parent->GetComponent<component::VertexList>();
                vlist->at(0).position = m_camera.ScreenToWorld(screenMouseCoord);
                m_scene.entities.emplace_back(parent);
                m_settings.drawStep = 0;
                m_settings.curDrawing = Settings::CUR_DRAWING::NONE_SHAPE;
            }
        }
        }
    }
}