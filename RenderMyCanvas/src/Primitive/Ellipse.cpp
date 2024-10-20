#include "pch.hpp"
#include "Ellipse.hpp"
#include "Utils/Log.hpp"

#include "Component/Material.hpp"
#include "Component/Artist.hpp"
#include "Component/DrawAlgo2D.hpp"
#include "Component/Transform.hpp"

#include "Algorithm/MidPointEllipse.hpp"

#include "Utils/CoordTransformer.hpp"

namespace RMC::primitive {
    void Ellipse::Draw(Ptr<const camera::Camera> camera)
    {
        auto trsf = GetComponent<component::Transform>();
        if (trsf == nullptr) { RMC_WARN("Transform component not found!"); return; }
        auto vertexList = GetComponent<component::VertexList>();
        if (vertexList == nullptr) { RMC_WARN("VertexList component not found!"); return; }
        auto imageRange = camera->GetViewportSize();
        auto artist = GetComponent<component::Artist>();
        if (artist == nullptr) { RMC_WARN("Artist component not found!"); return; }
        artist->imageRange = imageRange;
        auto drawAlgo = GetComponent<component::DrawAlgo2D>();
        glm::vec3 center = { 0.0f, 0.0f, 0.0f };
        float a = glm::distance(center, vertexList->at(0).position);
        float b = glm::distance(center, vertexList->at(1).position);

        if (drawAlgo->algo == nullptr) {
            for (float angle = 0.0F; angle < 360.0F; angle += 0.1F) {
                auto point = center + glm::vec3(a * cos(angle), b * sin(angle), 0.0F);
                point = trsf->TransformPoint(point);
                auto screenCoord = camera->WorldToScreen(point);
                artist->PutPoint(screenCoord);
            }
        } else {
            using Point = algorithm::BaseDraw2D::Point;
            glm::vec3 center = glm::vec3{ 0.0F, 0.0F, 0.0F };
            ////float a = glm::distance(center, vertexList->at(0).position);
            ////float b = glm::distance(center, vertexList->at(1).position);

            glm::vec3 worldScreenApex = trsf->TransformPoint(center);
            glm::vec3 worldXApex = trsf->TransformPoint(vertexList->at(0).position);
            glm::vec3 worldYApex = trsf->TransformPoint(vertexList->at(1).position);

            float a = glm::distance(camera->WorldToScreen(worldXApex), camera->WorldToScreen(worldScreenApex));
            float b = glm::distance(camera->WorldToScreen(worldYApex), camera->WorldToScreen(worldScreenApex));

            glm::vec2 screenCenter = camera->WorldToScreen(trsf->translate);

            utils::CoordTransformer ctrans{};
            ctrans.Set2DTransformMatrix(math::MatTranslate(screenCenter));

            std::function<Point(const Point&)> transformFunc =
                std::bind(&utils::CoordTransformer::Transform2D<Point>, &ctrans, std::placeholders::_1);

            drawAlgo->SetParams(
                {
                    {"a", a},
                    {"b", b},
                    {"put_point_func", artist->GetPutPointFunc<Point>()},
                    {"transform_func", transformFunc}
                }
            );
            drawAlgo->Draw();

            ctrans.Set2DTransformMatrix(math::MatFlip(0) * ctrans.MatTranslate(screenCenter));
            std::function<Point(const Point&)> transformFunc2 =
                std::bind(&utils::CoordTransformer::Transform2D<Point>, &ctrans, std::placeholders::_1);
            drawAlgo->Draw();

            ctrans.Set2DTransformMatrix(math::MatFlip(1) * ctrans.MatTranslate(screenCenter));
            drawAlgo->Draw();

            ctrans.Set2DTransformMatrix(math::MatFlip(0) * math::MatFlip(1) * ctrans.MatTranslate(screenCenter));
            drawAlgo->Draw();
        }
    }
} // namespace RMC::primitive