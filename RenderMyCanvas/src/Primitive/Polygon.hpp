#pragma once
#include "BasePrimitive.hpp"
#include "Algorithm/BaseDraw2D.hpp"

namespace RMC::primitive {
    class Polygon : public BasePrimitive
    {
    public:
        using MyBase = BasePrimitive;
        using BasePrimitive::GetComponent;
        using BasePrimitive::AddComponent;
        using BasePrimitive::AddComponents;
        using BasePrimitive::RemoveComponent;

        using Point = algorithm::BaseDraw2D::Point;

    public:
        explicit Polygon() : BasePrimitive(){
            this->AddComponent<component::Tag>("RMC::primitive::Polygon");
        }

        virtual void Draw(ConstPtr<camera::Camera> camera) override final;

        virtual float Hit(const renderer::Ray& ray, const component::Transform& trsf, const component::VertexList& vlist) const override final;
    };
}
