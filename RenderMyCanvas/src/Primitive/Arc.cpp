#include "pch.hpp"
#include "Arc.hpp"

#include "Component/Transform.hpp"
#include "Component/Artist.hpp"
#include "Component/VertexList.hpp"
#include "Component/OptionMap.hpp"

namespace RMC::primitive {
    void Arc::Draw(Ptr<const camera::Camera> camera)
    {
        auto transform = GetComponent<component::Transform>();
        if (transform == nullptr) { RMC_WARN("Transform component not found!"); return; }
        auto vertexList = GetComponent<component::VertexList>();
        if (vertexList == nullptr) { RMC_WARN("VertexList component not found!"); return; }
        auto imageRange = camera->GetViewportSize();
        auto artist = GetComponent<component::Artist>();
        if (artist == nullptr) { RMC_WARN("Artist component not found!"); return; }
        artist->imageRange = imageRange;

        glm::vec3 center = { 0.0f, 0.0f, 0.0f };
        const glm::vec3& start = vertexList->at(0).position;
        const glm::vec3& end = vertexList->at(1).position;
        float radius = glm::distance(center, start);

        float degree = glm::degrees(::acos(glm::dot(glm::normalize(start), glm::normalize(end))));

        auto optionMap = GetComponent<component::OptionMap>();
        if (optionMap != nullptr && optionMap->find("arc_type") != optionMap->end()) {
            if (optionMap->at("arc_type") == "major") {
                degree = 360.0f - degree;
                for (float d = 0.0f; d <= degree; d += 0.01f) {
                    auto point = center + glm::vec3{ radius * ::cos(math::Degree_to_Radian(d)), -radius * ::sin(math::Degree_to_Radian(d)), 0.0f };
                    point = transform->TransformPoint(point);
                    auto screenCoord = camera->WorldToScreen(point);
                    artist->PutPoint(screenCoord);
                }
                return;
            }
        } else {
            for (float d = 0.0f; d <= degree; d += 0.01f) {
                auto point = center + glm::vec3{ radius * ::cos(math::Degree_to_Radian(d)), radius * ::sin(math::Degree_to_Radian(d)), 0.0f };
                point = transform->TransformPoint(point);
                auto screenCoord = camera->WorldToScreen(point);
                artist->PutPoint(screenCoord);
            }
        }
    }
} // RMC::primitive