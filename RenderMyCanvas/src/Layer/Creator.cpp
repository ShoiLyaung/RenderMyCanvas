#include "pch.hpp"

#include "Layer.hpp"

#include "Primitive/LineSegment.hpp"
#include "Primitive/Ellipse.hpp"
#include "Primitive/Polygon.hpp"
#include "Primitive/Curve.hpp"
#include "Primitive/Parent.hpp"
#include "Primitive/ControlPoint.hpp"
#include "Primitive/Arc.hpp"
#include "Primitive/Sphere.hpp"

#include "Algorithm/ScanLineFill.hpp"
#include "Algorithm/BSplineCurve.hpp"
#include "Algorithm/BezierCurve.hpp"

#include "Component/ChildEntities.hpp"

namespace RMC::layer {
    template<>
    auto Layer::CreatePrimitive<primitive::LineSegment>() -> std::shared_ptr<primitive::BasePrimitive>
    {
        auto lineSeg = std::make_shared<primitive::LineSegment>();
        lineSeg->AddComponents<component::Transform, component::VertexList, component::DrawAlgo2D>();
        lineSeg->GetComponent<component::DrawAlgo2D>()->SetAlgorithm<algorithm::Bresenham>();
        auto vlist = lineSeg->GetComponent<component::VertexList>();
        vlist->resize(2);
        vlist->at(0).position = { 0.0f,0.0f,0.0f }; // Start
        vlist->at(1).position = { 0.0f,0.0f,0.0f }; // End
        component::Artist artist;
        artist.whichPrimitive = lineSeg;
        artist.buffPrimitivePerPixel = &m_renderer.GetBufferPrimitivePerPixel();
        artist.buffImage = &m_renderer.GetBufferImage();
        lineSeg->AddComponent<component::Artist>(artist);
        return lineSeg;
    }

    template<>
    auto Layer::CreatePrimitive<primitive::Ellipse>() -> std::shared_ptr<primitive::BasePrimitive>
    {
        auto ellipse = std::make_shared<primitive::Ellipse>();
        ellipse->AddComponents<component::Transform, component::VertexList, component::DrawAlgo2D>();
        auto vlist = ellipse->GetComponent<component::VertexList>();
        vlist->resize(4);
        vlist->at(0).position = { 0.0f, 0.0f, 0.0f }; // X-apex
        vlist->at(1).position = { 0.0f, 0.0f, 0.0f }; // Y-apex
        vlist->at(2).position = { 0.0f, 0.0f, 0.0f }; // Neg X-apex
        vlist->at(3).position = { 0.0f, 0.0f, 0.0f }; // Neg Y-apex
        component::Artist artist;
        artist.whichPrimitive = ellipse;
        artist.buffPrimitivePerPixel = &m_renderer.GetBufferPrimitivePerPixel();
        artist.buffImage = &m_renderer.GetBufferImage();
        artist.thickness = 3;
        ellipse->AddComponent<component::Artist>(artist);
        return ellipse;
    }

    template<>
    auto Layer::CreatePrimitive<primitive::Polygon>() -> std::shared_ptr<primitive::BasePrimitive>
    {
        auto polygon = std::make_shared<primitive::Polygon>();
        polygon->AddComponents<component::Transform, component::VertexList, component::DrawAlgo2D, component::Material>();
        polygon->GetComponent<component::DrawAlgo2D>()->SetAlgorithm<algorithm::ScanLineFill>();
        component::Artist artist;
        artist.whichPrimitive = polygon;
        artist.buffPrimitivePerPixel = &m_renderer.GetBufferPrimitivePerPixel();
        artist.buffImage = &m_renderer.GetBufferImage();
        polygon->AddComponent<component::Artist>(artist);
        return polygon;
    }

    template<>
    auto Layer::CreatePrimitive<primitive::Curve>() -> std::shared_ptr<primitive::BasePrimitive>
    {
        auto polygon = std::make_shared<primitive::Curve>();
        polygon->AddComponents<component::Transform, component::VertexList, component::DrawAlgo2D>();
        polygon->GetComponent<component::DrawAlgo2D>()->SetAlgorithm<algorithm::BSplineCurve>();
        component::Artist artist;
        artist.whichPrimitive = polygon;
        artist.buffPrimitivePerPixel = &m_renderer.GetBufferPrimitivePerPixel();
        artist.buffImage = &m_renderer.GetBufferImage();
        artist.thickness = 3;
        polygon->AddComponent<component::Artist>(artist);
        return polygon;
    }

    template<>
    auto Layer::CreatePrimitive<primitive::ControlPoint>() -> std::shared_ptr<primitive::BasePrimitive>
    {
        auto ctrlPoint = std::make_shared<primitive::ControlPoint>();
        ctrlPoint->AddComponents<component::TransformBinder, component::VertexBinder>();
        component::Artist artist;
        artist.whichPrimitive = ctrlPoint;
        artist.buffPrimitivePerPixel = &m_renderer.GetBufferPrimitivePerPixel();
        artist.buffImage = &m_renderer.GetBufferImage();
        artist.color = 0xFF00FF00;
        artist.thickness = 9;
        ctrlPoint->AddComponent<component::Artist>(artist);
        return ctrlPoint;
    }

    template<>
    auto Layer::CreatePrimitive<primitive::Parent>() -> std::shared_ptr<primitive::BasePrimitive>
    {
        auto parent = std::make_shared<primitive::Parent>();
        parent->AddComponents<component::Transform, component::VertexList, component::ChildEntities>();
        auto vlist = parent->GetComponent<component::VertexList>();
        vlist->resize(1);  // Center
        component::Artist artist;
        artist.whichPrimitive = parent;
        artist.buffPrimitivePerPixel = &m_renderer.GetBufferPrimitivePerPixel();
        artist.buffImage = &m_renderer.GetBufferImage();
        artist.thickness = 17;
        parent->AddComponent<component::Artist>(artist);
        return parent;
    }

    template<>
    auto Layer::CreatePrimitive<primitive::Arc>() -> std::shared_ptr<primitive::BasePrimitive>
    {
        auto arc = std::make_shared<primitive::Arc>();
        arc->AddComponents<component::Transform, component::VertexList, component::OptionMap>();
        auto vlist = arc->GetComponent<component::VertexList>();
        vlist->resize(2); // Start, End
        component::Artist artist;
        artist.whichPrimitive = arc;
        artist.buffPrimitivePerPixel = &m_renderer.GetBufferPrimitivePerPixel();
        artist.buffImage = &m_renderer.GetBufferImage();
        artist.thickness = 3;
        arc->AddComponent<component::Artist>(artist);
        return arc;
    }

    template<>
    auto Layer::CreatePrimitive<primitive::Sphere>() -> std::shared_ptr<primitive::BasePrimitive>
    {
        auto sphere = std::make_shared<primitive::Sphere>();
        sphere->AddComponents<component::Transform, component::Material, component::VertexList>();
        auto vlist = sphere->GetComponent<component::VertexList>();
        vlist->resize(6);
        vlist->at(0).position = { 1.0f, 0.0f, 0.0f };
        vlist->at(1).position = { -1.0f, 0.0f, 0.0f };
        vlist->at(2).position = { 0.0f, 1.0f, 0.0f };
        vlist->at(3).position = { 0.0f, -1.0f, 0.0f };
        vlist->at(4).position = { 0.0f, 0.0f, 1.0f };
        vlist->at(5).position = { 0.0f, 0.0f, -1.0f };
        return sphere;
    }
}