#pragma once
#include "BasePrimitive.hpp"

namespace RMC::primitive {
    class ControlPoint : public BasePrimitive
    {
    public:
        using MyBase = BasePrimitive;
        using BasePrimitive::GetComponent;
        using BasePrimitive::AddComponent;
        using BasePrimitive::AddComponents;
        using BasePrimitive::RemoveComponent;

    public:
        explicit ControlPoint() : MyBase()
        {
            AddComponent<::RMC::component::Tag>("RMC::primitive::ControlPoint");
        }

        virtual void Draw(Ptr<const camera::Camera>) override final;

        virtual float Hit(const renderer::Ray& ray, const component::Transform& trsf, const component::VertexList& vlist) const override final
        {
            return -1;
        }
    };
}