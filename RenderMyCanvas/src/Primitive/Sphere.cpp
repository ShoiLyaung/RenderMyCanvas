#include "pch.hpp"
#include "Sphere.hpp"
#include "Component/Transform.hpp"

namespace RMC::primitive {
    float Sphere::Hit(const renderer::Ray & ray, const component::Transform & trsf, const component::VertexList & vlist) const
    {
        ////glm::vec3 sphereCenter = trsf.TransformPoint(vlist.GetCentroid());
        glm::vec3 sphereCenter = trsf.translate;
        float sphereRadius = vlist[0].position[0];

        glm::vec3 origin = ray.origin - sphereCenter;

        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(origin, ray.direction);
        float c = glm::dot(origin, origin) - sphereRadius * sphereRadius;

        float discriminant = b * b - 4.0f * a * c;

        if (discriminant < 0.0f) {
            return -1;
        }

        float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);

        return closestT;
    }
}