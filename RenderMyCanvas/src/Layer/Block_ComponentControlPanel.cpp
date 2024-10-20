#include "pch.hpp"

#include "Layer.hpp"

#include "Component/Artist.hpp"
#include "Component/Transform.hpp"
#include "Component/VertexList.hpp"
#include "Component/ChildEntities.hpp"
#include "Component/OptionMap.hpp"

// Curve >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "Algorithm/BSplineCurve.hpp"
#include "Algorithm/BezierCurve.hpp"
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// Line Segment >>>>>>>>>>>>>>>>>>>>>>>>>>
#include "Algorithm/Bresenham.hpp"
#include "Algorithm/MidPointLine.hpp"
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#include "Algorithm/MidPointEllipse.hpp"

namespace RMC::layer {
    void Layer::Block_ComponentControlPanel()
    {
        if (m_selectedPrimitive == nullptr) { return; }
        auto nameOfSelectedPrimitive = m_selectedPrimitive->GetComponent<component::Tag>()->toString();

        if (nameOfSelectedPrimitive == "RMC::primitive::Arc") {
            auto& arc = m_selectedPrimitive;
            auto optionMap = arc->GetComponent<component::OptionMap>();
            static const char* arcType[] = { "minor", "major" };
            static int curArcType = 0;
            static int lastArcType = 0;
            ImGui::Combo("Arc Type", &curArcType, arcType, IM_ARRAYSIZE(arcType));
            if (curArcType != lastArcType) {
                if (curArcType == 0) {
                    optionMap->set("arc_type", "minor");
                } else if (curArcType == 1) {
                    optionMap->set("arc_type", "major");
                }
                lastArcType = curArcType;
            }
            ImGui::Separator();
        }

        auto selectedTrsf = m_selectedPrimitive->GetComponent<RMC::component::Transform>();
        if (selectedTrsf != nullptr && nameOfSelectedPrimitive != "RMC::primitive::Parent")
        {
            // Set translate
            ImGui::DragFloat3("Translate", &selectedTrsf->translate[0], 0.01f);

            // Set rotation
            // ||Bug|| [2023/Oct/21]
            //    When eulerAngles[1] is 90 or -90, the rotation will be wrong.
            Ref<glm::quat> originRotation = selectedTrsf->rotation;
            glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(originRotation));
            float rotateSpeed = 1.0f;
            ImGui::DragFloat3("Rotation", &eulerAngles[0]);
            originRotation = glm::quat(glm::radians(glm::vec3{ eulerAngles[0], eulerAngles[1], eulerAngles[2] }));

            // Set scale
            ImGui::DragFloat3("Scale", &selectedTrsf->scale[0], 0.1f);

            ImGui::Separator();
        }

        auto childVCpnt = m_selectedPrimitive->GetComponent<RMC::component::ChildEntities>();
        if (childVCpnt != nullptr && nameOfSelectedPrimitive == "RMC::primitive::Parent") {
            auto parentVlist = m_selectedPrimitive->GetComponent<component::VertexList>();
            auto& ctrlTrsf = childVCpnt->controlTransform; // Transform for controlling children
            // Set translate
            ImGui::DragFloat3("Ctrl Translate", &ctrlTrsf.translate[0], 0.01);

            // Set rotation
            // ||Bug|| [2023/Oct/21]
            //    When eulerAngles[1] is 90 or -90, the rotation will be wrong.
            Ref<glm::quat> originRotation = ctrlTrsf.rotation;
            glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(originRotation));
            float rotateSpeed = 1.0f;
            ImGui::DragFloat3("Ctrl Rotation", &eulerAngles[0], 0.5f);
            originRotation = glm::quat(glm::radians(glm::vec3{ eulerAngles[0], eulerAngles[1], eulerAngles[2] }));

            // Set scale
            ImGui::DragFloat3("Ctrl Scale", &ctrlTrsf.scale[0], 0.1f);

            childVCpnt->UpdateChildPrimitiveTransform(ctrlTrsf, parentVlist->at(0).position);

            ImGui::Separator();
        }

        auto artist = m_selectedPrimitive->GetComponent<RMC::component::Artist>();
        if (artist != nullptr) {
            auto originColor = utils::uint32_t_to_RGBA(artist->color);
            float color[4] = { originColor[0], originColor[1], originColor[2], originColor[3] };
            ImGui::ColorEdit4("Color 1", color);
            Ref<uint32_t> thickness = artist->thickness;
            ImGui::InputScalar("Thickness", ImGuiDataType_U32, &thickness);
            artist->color = utils::RGBA_to_uint32_t({ color[0], color[1], color[2], color[3] });

            ImGui::Separator();
        }

        auto drawAlgo2d = m_selectedPrimitive->GetComponent<RMC::component::DrawAlgo2D>();
        if (drawAlgo2d != nullptr) {
            if (nameOfSelectedPrimitive == "RMC::primitive::Curve") {
                // Show a list to choose use Bezier or BSpline
                static const char* items[] = { "BSpline", "Bezier" };
                static int item_current = 0;
                static int item_last = 0;  // BSpline
                ImGui::Combo("Curve Type", &item_current, items, IM_ARRAYSIZE(items));
                if (item_current != item_last) {
                    if (item_current == 0) {
                        drawAlgo2d->SetAlgorithm<algorithm::BSplineCurve>();
                    } else if (item_current == 1) {
                        drawAlgo2d->SetAlgorithm<algorithm::BezierCurve>();
                    }
                    item_last = item_current;
                }
                if (item_current == 0) {
                    static uint32_t order = 3;
                    ImGui::InputScalar("order", ImGuiDataType_U32, &order);
                    drawAlgo2d->SetParams({ {"order", uint64_t(order)} });
                }
            }
            if (nameOfSelectedPrimitive == "RMC::primitive::LineSegment") {
                static const char* items[] = { "Bresenhame", "Mid-Point" };
                static int item_current = 0;
                static int item_last = 0;  // Bresemhame
                ImGui::Combo("LineSeg Algo", &item_current, items, IM_ARRAYSIZE(items));
                if (item_current != item_last) {
                    if (item_current == 0) {
                        drawAlgo2d->SetAlgorithm<algorithm::Bresenham>();
                    } else if (item_current == 1) {
                        drawAlgo2d->SetAlgorithm<algorithm::MidPointLine>();
                    }
                    item_last = item_current;
                }

                static Ptr<const char> modes[] = { "Plain", "Dash", "Dot" };
                static int mode_current = 0;
                static int mode_last = 0;  // Plain
                ImGui::Combo("LineSeg Mode", &mode_current, modes, IM_ARRAYSIZE(modes));
                if (mode_current != mode_last) {
                    drawAlgo2d->SetParams({ {"mode", uint8_t(mode_current)} });
                    mode_last = mode_current;
                }
            }
            if (nameOfSelectedPrimitive == "RMC::primitive::Ellipse") {
                static const char* items[] = { "Generate", "Mid-Point" };
                static int item_current = 0;
                static int item_last = 0;  // Generate
                ImGui::Combo("Ellipse Algo", &item_current, items, IM_ARRAYSIZE(items));
                if (item_current != item_last) {
                    if (item_current == 0) {
                        drawAlgo2d->RemoveAlgorithm();
                    } else if (item_current == 1) {
                        drawAlgo2d->SetAlgorithm<algorithm::MidPointEllipse>();
                    }
                    item_last = item_current;
                }
            }

            ImGui::Separator();
        }
    }
}