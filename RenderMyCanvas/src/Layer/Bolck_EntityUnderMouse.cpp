#include "pch.hpp"

#include "Layer.hpp"
#include "Walnut/Input/Input.h"
#include "Component/Transform.hpp"
#include "Component/VertexList.hpp"
#include "Component/ChildEntities.hpp"

#include "Primitive/ControlPoint.hpp"

namespace RMC::layer {
    void Layer::Block_PrimitiveUnderMouse()
    {
        static std::shared_ptr<primitive::BasePrimitive> lastSelectedPrimitive = nullptr;
        static std::shared_ptr<primitive::BasePrimitive> underControlled = nullptr;
        static std::string nameOfUnderControlled = "None";
        static std::string nameOfSelectedPrimitive = "None";
        static std::string nameOfPrimitiveUnderMouse = "None";
        static glm::vec3 originCentroid{};

        // Show What is Under Mouse >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        glm::vec2 viewportMouseCoord = this->GetViewportMouseCoord();
        auto& buffPrimitivePerPixel = m_renderer.GetPrimitivePerPixel();
        std::shared_ptr<primitive::BasePrimitive> entityUnderMouse = nullptr;
        // If buffPrimitivePerPixel is not empty and mouse is in viewport
        if (!buffPrimitivePerPixel.empty() && this->IsInViewport(viewportMouseCoord)) {
            // Get the shared pointer to primitive under mouse
            entityUnderMouse = buffPrimitivePerPixel.at(utils::To1DIndex(viewportMouseCoord, m_viewportWidth));
            // Set `nameUnderMouse` to the primitive's name if there is an primitive under mouse
            if (entityUnderMouse != nullptr) {
                auto tag = entityUnderMouse->GetComponent<component::Tag>();
                nameOfPrimitiveUnderMouse = tag->toString();
            }
        }
        ImGui::Text("Primitive Under Mouse: %s", nameOfPrimitiveUnderMouse.c_str());
        // Show What is Under Mouse <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        // Select Primitive with Mouse Left-Button >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        if (m_selectedPrimitive != nullptr) {
            nameOfSelectedPrimitive = m_selectedPrimitive->GetComponent<component::Tag>()->toString();
        }
        if (m_settings.curDrawing != Settings::CUR_DRAWING::NONE_SHAPE) {
            m_settings.selectStep = 0;  // Reset `m_settings.selectStep`
            m_selectedPrimitive = nullptr; // Reset `m_selectedPrimitive`
            m_scene.controlPoints.clear();
        } else {
            switch (m_settings.selectStep) {
            // CASE 0 - Check Mouse Left-Button Down and Update `m_selectedPrimitive` >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            case 0: {
                // If mouse left button down an there is an primitive under mouse
                if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                    if (entityUnderMouse == nullptr) { break; }

                    if (nameOfPrimitiveUnderMouse == "RMC::primitive::ControlPoint") {
                        if (nameOfSelectedPrimitive != "RMC::primitive::ControlPoint") {
                            underControlled = m_selectedPrimitive;
                            nameOfUnderControlled = nameOfSelectedPrimitive;
                        }
                        m_settings.selectStep = 2; // Activate CASE 2
                    } else {
                        m_settings.selectStep = 1; // Activate CASE 1
                    }

                    if (lastSelectedPrimitive != entityUnderMouse) {
                        m_selectedPrimitive = entityUnderMouse; // Update selected primitive
                        nameOfSelectedPrimitive = nameOfPrimitiveUnderMouse;
                        lastSelectedPrimitive = m_selectedPrimitive; // Update last selected primitive
                    }

                    if (nameOfSelectedPrimitive == "RMC::primitive::Parent") {
                        auto childEntitiesCpnt = m_selectedPrimitive->GetComponent<component::ChildEntities>();
                        auto& originTrsfs = childEntitiesCpnt->originTransforms;
                        auto& childEntities = childEntitiesCpnt->childEntities;
                        auto& ctrlTrsf = childEntitiesCpnt->controlTransform;
                        // Update every origin transform of child entities
                        for (uint64_t i = 0; i < originTrsfs.size(); ++i) {
                            originTrsfs[i] = *(childEntities[i]->GetComponent<component::Transform>());
                        }
                        // Reset control transform
                        ctrlTrsf = component::Transform();
                    }
                }
                break; // Break CASE 0
            }
            // CASE 0 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            // CASE 1 - Check Mouse Left-Button Up and Create Control Point >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            case 1: {
                if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left))
                {
                    if (lastSelectedPrimitive != entityUnderMouse) {
                        m_settings.selectStep = 0;
                        break; // Break CASE 1
                    }
                    m_selectedPrimitive = entityUnderMouse;
                    lastSelectedPrimitive = m_selectedPrimitive;
                    // Clear `m_scene.controlPoints`
                    m_scene.controlPoints.clear();
                    // Get vlist of `m_selectedPrimitive`
                    auto vlist = m_selectedPrimitive->GetComponent<component::VertexList>();
                    // Get transform of `m_selectedPrimitive`
                    auto trsf = m_selectedPrimitive->GetComponent<component::Transform>();
                    // Draw transformed vertexes to screen
                    for (uint64_t i = 0; i < vlist->vertices.size(); ++i) {
                        auto ctrlPoint = this->CreatePrimitive<primitive::ControlPoint>();
                        auto trsfIter = ctrlPoint->GetComponent<component::TransformBinder>();
                        trsfIter->transform_ptr = trsf;
                        auto vertexBinder = ctrlPoint->GetComponent<component::VertexBinder>();
                        vertexBinder->vertex_ptr = &vlist->vertices.at(i);
                        m_scene.controlPoints.emplace_back(ctrlPoint);
                    }
                    // Record Origin centroid
                    originCentroid = vlist->GetCentroid();
                    // Activate CASE 0
                    m_settings.selectStep = 0;
                }
                break;
            }
            // CASE 1 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            // CASE 2 - Drag Control Point >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            case 2: {
                if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                    // Get binder to transform of being-controlled vertex
                    auto transformBinder = m_selectedPrimitive->GetComponent<component::TransformBinder>();
                    // Get binder to vertex of being-controlled vertex
                    auto vertexBinder = m_selectedPrimitive->GetComponent<component::VertexBinder>();
                    // Calculate the distance from the control point to camera
                    glm::vec3 ctrlPointWorldPosition = transformBinder->transform_ptr->TransformPoint((vertexBinder->vertex_ptr)->position);
                    float ctrlPointFowardDistance = glm::dot((ctrlPointWorldPosition - m_camera.GetPosition()), m_camera.GetForwardDirection());
                    // Caclulate mouse world position
                    glm::vec3 mouseWorldPosition = m_camera.ScreenToWorld(
                        this->GetViewportMouseCoord(), ctrlPointFowardDistance
                    );
                    // Calculate the inverse transformed mouse world position
                    glm::vec3 inverseTransformedMouseWorldPosition = transformBinder->transform_ptr->InverseTransformPoint(mouseWorldPosition);
                    // Update the position of being-controlled vertex
                    vertexBinder->vertex_ptr->position = inverseTransformedMouseWorldPosition;
                } else {
                    m_settings.selectStep = 3; // Activate CASE 0
                }
                break;
            }
            // CASE 2 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            // CASE 3 - Drag Control Point Finished >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            case 3: {
                if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                    // Get vlist of `m_selectedPrimitive`
                    auto vlist = underControlled->GetComponent<component::VertexList>();
                    // Get transform of `m_selectedPrimitive`
                    auto trsf = underControlled->GetComponent<component::Transform>();
                    // Get inverserotated centroid of vlist
                    glm::vec3 centroid = vlist->GetCentroid();
                    // Translate every vertex to centroid
                    for (auto& vertex : vlist->vertices) {
                        vertex.position -= centroid;
                    }
                    // Adjust translate to ensure the object stays in the same place
                    trsf->translate += glm::vec3(trsf->GetRotationMat() * glm::vec4(centroid, 1.0f));
                    m_settings.selectStep = 0;
                }
                break;
            }
            // CASE 3 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            default: {
                break;
            }
            }
        }

        ImGui::Text("Selected Primitive: %s", nameOfSelectedPrimitive.c_str());
        // Select Primitive with Mouse Left-Button <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        // If DEL is pressed, delete selected primitive >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Delete)) {
            auto it = std::ranges::find(m_scene.entities, m_selectedPrimitive);
            if (it != m_scene.entities.end()) {
                m_scene.controlPoints.clear();
                m_scene.entities.erase(it);
                m_selectedPrimitive = nullptr;
                lastSelectedPrimitive = nullptr;
            }
        }
        // If DEL is pressed >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // If ESC is pressed, unselect primitive >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Escape)) {
            lastSelectedPrimitive = nullptr;
            m_scene.controlPoints.clear();
            m_selectedPrimitive = nullptr;
        }
        // If ESC is pressed <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        // If parent primitive is selected, add child primitive >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        if (m_selectedPrimitive == nullptr) { return; }
        if (nameOfSelectedPrimitive != "RMC::primitive::Parent") { return; }
        // Get childEntities component
        auto childEntitiesCpnt = m_selectedPrimitive->GetComponent<component::ChildEntities>();
        switch (m_settings.childSelectStep) {
        case 0: {
            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Right)) {
                // Break if there is no primitive under mouse
                if (entityUnderMouse == nullptr) { break; }
                // Break if the primitive under mouse is parent
                if (nameOfPrimitiveUnderMouse == "RMC::primitive::Parent") { break; }
                // Insert the primitive under mouse to `childEntitiesCpnt->childEntities` if it has not been added to the parent
                if (std::ranges::find(childEntitiesCpnt->childEntities, entityUnderMouse)
                    == childEntitiesCpnt->childEntities.end())
                {
                    // Add the under-mouse primitive to parent's child entities
                    childEntitiesCpnt->childEntities.emplace_back(entityUnderMouse);
                    // Get transform of under-mouse primitive
                    auto childTransform = entityUnderMouse->GetComponent<component::Transform>();
                    // Get transform of parent primitive
                    auto parentTransform = m_selectedPrimitive->GetComponent<component::Transform>();
                    // Add the origin transform of under-mouse primitive to `childEntitiesCpnt->buffTransforms`
                    childEntitiesCpnt->originTransforms.push_back(*childTransform);
                }
                m_settings.childSelectStep = 1;
            }
            break;
        }
        case 1: {
            if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Right)) {
                m_settings.childSelectStep = 0;
            }
        }
        }
        // If parent primitive is selected, add child primitive <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }
}