#include "pch.hpp"
#include "Polygon.hpp"
#include "Component/VertexList.hpp"
#include "Component/Artist.hpp"
#include "Component/Mode.hpp"
#include "Component/DrawAlgo2D.hpp"
#include "Component/Transform.hpp"

namespace RMC::primitive {
    void Polygon::Draw(ConstPtr<camera::Camera> camera)
    {
        auto transform = GetComponent<component::Transform>();
        if (transform == nullptr) { RMC_WARN("Transform component not found!"); return; }
        auto vertexList = GetComponent<component::VertexList>();
        if (vertexList == nullptr) { RMC_WARN("VertexList component not found!"); return; }
        if (vertexList->size() < 3) { return; }
        auto artist = GetComponent<component::Artist>();
        if (artist == nullptr) { RMC_WARN("Artist component not found!"); return; }
        artist->imageRange = camera->GetViewportSize();
        auto drawAlgo = GetComponent<component::DrawAlgo2D>();
        if (drawAlgo == nullptr) { RMC_WARN("DrawAlgo component not found!"); return; }

        auto imageRange = camera->GetViewportSize();

        auto transformedVertexList = transform->TransformVertexList(*vertexList);

        std::vector<Point> screenCoords(vertexList->vertices.size());
        for (uint64_t i = 0; i < vertexList->vertices.size(); ++i) {
            auto screenCoord = camera->WorldToScreen(transformedVertexList.vertices[i].position);
            screenCoords[i] = Point{ screenCoord[0], screenCoord[1] };
        }

        drawAlgo->SetParams(
            {
                { "vertices", screenCoords },
                { "image_height", imageRange[1]},
                { "image_width", imageRange[0] },
                { "put_point_func", artist->GetPutPointFunc<Point>()}
            }
        );

        drawAlgo->Draw();
    }

    float Polygon::Hit(const renderer::Ray & ray, const component::Transform & trsf, const component::VertexList & vlist) const
    {
        float closestT = std::numeric_limits<float>::max();
        if (vlist.size() < 3) return -1;

        glm::vec3 transformedOrigin = ray.origin - trsf.translate;

        // Hit every triangle
        for (uint64_t i = 0; i < vlist.size() - 2; ++i) {
            float distance = -1;
            glm::vec2 bary{};
            bool is_intersect = glm::intersectRayTriangle(
                transformedOrigin, ray.direction, 
                vlist[i].position, vlist[i + 1].position, vlist[i + 2].position, 
                bary, distance
            );
            if (!is_intersect) return std::numeric_limits<float>::max();
            if (distance < closestT) closestT = distance;
        }

        return closestT;
    }
}