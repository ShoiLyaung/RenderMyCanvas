#pragma once
#include "BasePrimitive.hpp"
#include "Camera/Camera.hpp"
#include "Renderer/Ray.hpp"

namespace RMC::primitive {
    class Sphere : public BasePrimitive
    {
    public:
        using MyBase = BasePrimitive;
        using BasePrimitive::GetComponent;
        using BasePrimitive::AddComponent;
        using BasePrimitive::AddComponents;
        using BasePrimitive::RemoveComponent;

    public:
        explicit Sphere() : MyBase()
        {
            this->AddComponent<::RMC::component::Tag>("RMC::primitive::Ellipse");
        }

        virtual void Draw(Ptr<const camera::Camera> camera) override final {};

        // @brief Get distance from ray origin to the primitive; If not hit, return -1.
        virtual float Hit(const renderer::Ray& ray, const component::Transform& trsf, const component::VertexList& vlist) const override final;
    };
} // namespace RMC::primitive