#pragma once
#include "Algorithm/Bresenham.hpp"
#include "BasePrimitive.hpp"

namespace RMC::primitive {
    class LineSegment : public BasePrimitive
    {
    public:
        using MyBase = BasePrimitive;
        using BasePrimitive::GetComponent;
        using BasePrimitive::AddComponent;
        using BasePrimitive::AddComponents;
        using BasePrimitive::RemoveComponent;

        using Point = algorithm::Bresenham::Point;

    public:
        explicit LineSegment()
        {
            this->AddComponent<component::Tag>("RMC::primitive::LineSegment");
        }


        virtual void Draw(
            ConstPtr<camera::Camera> camera
        ) override final;

        virtual float Hit(const renderer::Ray& ray, const component::Transform& trsf, const component::VertexList& vlist) const override final
        {
            return -1;
        }

    };
} // namespace RMC