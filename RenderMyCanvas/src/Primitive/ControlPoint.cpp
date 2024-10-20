#include "pch.hpp"
#include "BasePrimitive.hpp"
#include "ControlPoint.hpp"

#include "Component/Transform.hpp"
#include "Component/VertexList.hpp"
#include "Component/Artist.hpp"

namespace RMC::primitive {
    void RMC::primitive::ControlPoint::Draw(Ptr<const camera::Camera> camera)
    {
        auto transformBinder = GetComponent<component::TransformBinder>();
        if (transformBinder == nullptr) { RMC_WARN("TransformBinder component not found!"); return; }
        auto vertexBinder = GetComponent<component::VertexBinder>();
        if (vertexBinder == nullptr) { RMC_WARN("VertexBinder component not found!"); return; }
        auto artist = GetComponent<component::Artist>();
        if (artist == nullptr) { RMC_WARN("Artist component not found!"); return; }
        artist->imageRange = camera->GetViewportSize();
        auto transformedVertex = transformBinder->transform_ptr->TransformVertex(*(vertexBinder->vertex_ptr));
        artist->PutPoint(camera->WorldToScreen(transformedVertex.position));
    }
}