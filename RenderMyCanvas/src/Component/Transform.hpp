#pragma once

#include "BaseComponent.hpp"
#include "VertexList.hpp"

namespace RMC::component {
    class Transform : public BaseComponent
    {
        using MyBase = BaseComponent;
    public:
        explicit Transform(
            const glm::vec3& t = glm::vec3{ 0.0f },
            const glm::quat& r = glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f },
            const glm::vec3& s = glm::vec3{ 1.0f }
        ) : MyBase(), translate(t), rotation(r), scale(s) {}

        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::Transform";
        }

        virtual ~Transform() override {}

        Vertex TransformVertex(const Vertex& vertex) const
        {
            Vertex result = vertex;
            result.position = TransformPoint(vertex.position);
            return result;
        }

        Vertex InverseTransformVertex(const Vertex& vertex) const
        {
            Vertex result = vertex;
            result.position = InverseTransformPoint(vertex.position);
            return result;
        }

        template<typename Point3D = glm::vec3>
            requires IsPoint3D<Point3D>
        Point3D TransformPoint(const Point3D& point) const
        {
            return glm::vec3(GetTranslateMat() * GetRotationMat() * GetScaleMat() * glm::vec4(glm::vec3(point), 1.0f));
        }

        glm::vec3 InverseTransformPoint(const glm::vec3& point) const {
            glm::mat4 inverseTranslate = glm::inverse(glm::translate(glm::mat4(1.0f), translate));
            glm::mat4 inverseScale = glm::inverse(glm::scale(glm::mat4(1.0f), scale));
            ////glm::mat4 inverseRotation = glm::inverse(glm::mat4_cast(rotation));
            glm::mat4 inverseRotation = glm::inverse(glm::mat4_cast(rotation));
            glm::mat4 transformMat = inverseScale * inverseRotation * inverseTranslate;
            ////glm::mat4 transformMat = inverseTranslate * inverseRotation * inverseScale;
            return glm::vec3(transformMat * glm::vec4(point, 1.0f));
        }

        VertexList TransformVertexList(const VertexList& vertexList) const
        {
            VertexList result;

            for (const auto& vertex : vertexList.vertices)
            {
                glm::vec3 transformedPosition = TransformPoint(vertex.position);
                result.emplace_back(transformedPosition);
            }

            return  result;
        }

        glm::mat4 GetTranslateMat() const { return glm::translate(glm::mat4(1.0f), translate); }
        glm::mat4 GetRotationMat() const { return glm::toMat4(rotation); }
        glm::mat4 GetScaleMat() const { return glm::scale(glm::mat4(1.0f), scale); }

        bool operator==(Ref<const Transform> rhs) const
        {
            return translate == rhs.translate && rotation == rhs.rotation && scale == rhs.scale;
        }

        bool operator!=(Ref<const Transform> rhs) const
        {
            return translate != rhs.translate || rotation != rhs.rotation || scale != rhs.scale;
        }

        glm::vec3 translate;
        glm::quat rotation;
        glm::vec3 scale;
    };

    struct TransformBinder : public BaseComponent
    {
        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::TransformBinder";
        }

        std::shared_ptr<Transform> transform_ptr;
    };
} // namespace RMC::component