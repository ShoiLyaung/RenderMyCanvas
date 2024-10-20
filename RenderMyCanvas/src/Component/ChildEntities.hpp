#pragma once
#include "Primitive/BasePrimitive.hpp"
#include "Transform.hpp"
#include "BaseComponent.hpp"

namespace RMC::component {
    class ChildEntities : public BaseComponent
    {
    public:
        explicit ChildEntities() = default;

        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::ChildEntities";
        }

        void UpdateChildPrimitiveTransform(Ref<const Transform> ctrlTrsf, Ref<const glm::vec3> pivot)
        {
            for (uint64_t i = 0; i < originTransforms.size(); ++i) {
                auto childTrsf = childEntities[i]->GetComponent<component::Transform>();
                const auto& originTrsf = originTransforms[i];

                glm::mat4 originTranslate = glm::translate(glm::mat4(1.0f), originTrsf.translate);
                glm::mat4 originRotation = glm::toMat4(glm::quat{ 1.0f, 0,0,0 });
                glm::mat4 originScale = glm::scale(glm::mat4(1.0f), originTrsf.scale);
                glm::mat4 ctrlTranslate = glm::translate(glm::mat4(1.0f), ctrlTrsf.translate);
                glm::mat4 ctrlRotation = glm::toMat4(ctrlTrsf.rotation);
                glm::mat4 ctrlScale = glm::scale(glm::mat4(1.0f), ctrlTrsf.scale);
                glm::mat4 pivotTranslate = glm::translate(glm::mat4(1.0f), pivot);
                glm::mat4 pivotInverseTranslate = glm::translate(glm::mat4(1.0f), -pivot);
                // [Note] Transform is applied from right to left
                glm::mat4 transform =
                    pivotTranslate * ctrlRotation * pivotInverseTranslate *
                    ctrlTranslate * originTranslate * originRotation *
                    originScale * ctrlScale;
                ////glm::mat4 transform = ctrlTranslate * originTranslate * originRotation * ctrlScale * originScale;
                childTrsf->translate = glm::vec3(transform[3]);
                glm::mat3 rotation = glm::mat3(transform);
                childTrsf->rotation = glm::quat_cast(transform);
                childTrsf->scale = glm::vec3(glm::length(transform[0]), glm::length(transform[1]), glm::length(transform[2]));
            }
        }

        void UpdateBuffEntitiesTransform(Ref<const Transform> parentTrsf)
        {
        }

    public:
        std::vector<std::shared_ptr<primitive::BasePrimitive>> childEntities;
        std::vector<component::Transform>                originTransforms;
        component::Transform                             controlTransform;
    };
}