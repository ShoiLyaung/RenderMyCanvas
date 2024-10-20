#include "pch.hpp"
#include "Curve.hpp"
#include "Component/Transform.hpp"
#include "Component/Material.hpp"
#include "Component/VertexList.hpp"
#include "Component/Artist.hpp"
#include "Component/DrawAlgo2D.hpp"
#include "Algorithm/BezierCurve.hpp"
#include "Algorithm/BSplineCurve.hpp"
#include "Component/Mode.hpp"

namespace RMC::primitive {
    void Curve::Draw(Ptr<const camera::Camera> camera)
    {
        auto transform = GetComponent<component::Transform>();
        if (transform == nullptr) { RMC_WARN("Transform component not found!"); return; }
        auto vertexList = GetComponent<component::VertexList>();
        if (vertexList == nullptr) { RMC_WARN("VertexList component not found!"); return; }
        if (vertexList->size() < 2) { return; }
        auto artist = GetComponent<component::Artist>();
        if (artist == nullptr) { RMC_WARN("Artist component not found!"); return; }
        artist->imageRange = camera->GetViewportSize();
        auto drawAlgo = GetComponent<component::DrawAlgo2D>();
        if (drawAlgo == nullptr) { RMC_WARN("DrawAlgo component not found!"); return; }
        auto mode = this->GetComponent<component::Mode>();

        auto transformedVertexList = transform->TransformVertexList(*vertexList);

        std::vector<component::DrawAlgo2D::Point> points{};
        for (const auto& vertex : transformedVertexList.vertices) {
            glm::vec2 screenCoord = camera->WorldToScreen(vertex.position);
            points.emplace_back(component::DrawAlgo2D::Point{ screenCoord.x, screenCoord.y });
        }

        drawAlgo->SetParams(
            {
                {"control_points", points},
                {"put_point_func", artist->GetPutPointFunc<algorithm::BaseDraw2D::Point>()}
            }
        );

        drawAlgo->Draw();
    }
}