#pragma once
#include "BasePrimitive.hpp"

namespace RMC::primitive {
    class Parent : public BasePrimitive
    {
    public:
        explicit Parent()
        {
            AddComponent<::RMC::component::Tag>("RMC::primitive::Parent");
        }

        virtual void Draw(ConstPtr<camera::Camera> camera) override final;

        virtual float Hit(const renderer::Ray& ray, const component::Transform& trsf, const component::VertexList& vlist) const override final
        {
            return -1;
        }
    };
}