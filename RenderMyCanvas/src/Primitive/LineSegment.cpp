#include "pch.hpp"

#include "LineSegment.hpp"
#include "Utils/Log.hpp"
#include "Component/Material.hpp"
#include "Component/OptionMap.hpp"
#include "Component/Artist.hpp"
#include "Component/DrawAlgo2D.hpp"
#include "Component/Transform.hpp"
#include "Utils/CoordTransformer.hpp"

namespace RMC::primitive {
    void LineSegment::Draw(ConstPtr<camera::Camera> camera)
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

        auto imageRange = camera->GetViewportSize();

        auto transformedVertexList = transform->TransformVertexList(*vertexList);
        auto start = camera->WorldToScreen(transformedVertexList[0].position);
        auto end = camera->WorldToScreen(transformedVertexList[1].position);

        if (start.x > end.x) { std::swap(start, end); }

        static auto ctranser = utils::CoordTransformer();
        static auto inverseCtranser = utils::CoordTransformer();

        if (end.x - start.x < 1.0f) {
            if (end.y < start.y) { std::swap(start, end); }
            ctranser.Set2DTransformMatrix(ctranser.MatFlip(2));
            inverseCtranser.Set2DTransformMatrix(ctranser.MatFlip(2));
        } else {
            float k = (end.y - start.y) / (end.x - start.x);
            if (k >= 1.0) {
                ctranser.Set2DTransformMatrix(ctranser.MatFlip(2));
                inverseCtranser.Set2DTransformMatrix(ctranser.MatFlip(2));
            } else if (k < -1) {
                ctranser.Set2DTransformMatrix(ctranser.MatFlip(1) * ctranser.MatFlip(2));
                inverseCtranser.Set2DTransformMatrix(ctranser.MatFlip(2) * ctranser.MatFlip(1));
            } else if (k < 0 && k >= -1) {
                ctranser.Set2DTransformMatrix(ctranser.MatFlip(1));
                inverseCtranser.Set2DTransformMatrix(ctranser.MatFlip(1));
            } else if (k >= 0.0 && k < 1.0) {
                ctranser.Set2DTransformMatrix(math::I3);
                inverseCtranser.Set2DTransformMatrix(math::I3);
            } else {
                throw std::runtime_error(std::format("Unexpected k: {}", k));
            }
        }

        std::function<Point(const Point&)> transformFunc
            = std::bind(&utils::CoordTransformer::Transform2D<Point>, &ctranser, std::placeholders::_1);
        std::function<Point(const Point&)> inverseTransformFunc
            = std::bind(&utils::CoordTransformer::Transform2D<Point>, &inverseCtranser, std::placeholders::_1);

        drawAlgo->SetParams(
            {
                {"start_point", Point{ start.x, start.y }},
                {"end_point", Point{ end.x, end.y }},
                {"transform_func", transformFunc},
                {"inverse_transform_func", inverseTransformFunc},
                {"put_point_func", artist->GetPutPointFunc<Point>()},
                {"image_width", imageRange[0]},
                {"image_height", imageRange[1]}
            }
        );

        drawAlgo->Draw();
    }
} // namespace RMC::primitive