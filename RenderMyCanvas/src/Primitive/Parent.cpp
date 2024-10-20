#include "pch.hpp"

#include "Parent.hpp"
#include "Component/Transform.hpp"
#include "Component/VertexList.hpp"
#include "Component/ChildEntities.hpp"
#include "Component/Artist.hpp"

namespace RMC::primitive {
    void Parent::Draw(ConstPtr<camera::Camera> camera)
    {
        auto trsf = GetComponent<component::Transform>();
        if (trsf == nullptr) { RMC_WARN("Transform component not found!"); return; }
        auto vlist = GetComponent<component::VertexList>();
        if (vlist == nullptr) { RMC_WARN("VertexList component not found!"); return; }
        auto artist = GetComponent<component::Artist>();
        if (artist == nullptr) { RMC_WARN("Artist component not found!"); return; }
        artist->imageRange = camera->GetViewportSize();
        artist->PutPoint(camera->WorldToScreen(trsf->TransformPoint(vlist->at(0).position)));

        const auto& childEntities = GetComponent<component::ChildEntities>()->childEntities;
        for (const auto& child : childEntities) {
            auto childVlist = child->GetComponent<component::VertexList>();
            if (childVlist == nullptr) { RMC_WARN("VertexList component not found!"); continue; }
            auto childTrsf = child->GetComponent<component::Transform>();
            artist->PutPoint(camera->WorldToScreen(childTrsf->TransformPoint(childVlist->GetCentroid())));
        }
    }
}