#pragma once
#include "BasePrimitive.hpp"
#include "Camera/Camera.hpp"

namespace RMC::primitive {
    class Arc : public BasePrimitive
    {
    public:
        using MyBase = BasePrimitive;
        using BasePrimitive::GetComponent;
        using BasePrimitive::AddComponent;
        using BasePrimitive::AddComponents;
        using BasePrimitive::RemoveComponent;

    public:
        explicit Arc() : MyBase()
        {
            this->AddComponent<::RMC::component::Tag>("RMC::primitive::Arc");
        }

        virtual void Draw(Ptr<const camera::Camera> camera) override final;

        virtual float Hit(const renderer::Ray& ray, const component::Transform& trsf, const component::VertexList& vlist) const override final
        {
            return -1;
        }
    };
} // namespace RMC::primitive