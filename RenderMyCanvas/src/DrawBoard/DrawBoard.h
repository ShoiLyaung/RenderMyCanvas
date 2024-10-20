//#pragma once
//#include "Algorithms/SeedFill.h"
//#include "Algorithms/ContourExtraction.h"
//#include "Algorithms/PolygonScanlineFill.h"
//#include "Primitive/Arc.h"
//#include "Primitive/Circle.h"
//#include "Primitive/Ellipse.h"
//#include "Primitive/Line.h"
//#include "Primitive/Primitive.h"
//#include "Renderer/Renderer.h"
//#include <glm/glm.hpp>
//#include <vector>
//#include <iostream>
//
//namespace RMC
//{
//enum class DrawingMode
//{
//    None,
//    Line,
//    Circle_CenterRadius,
//    Circle_Diameter,
//    Ellipse_Foci,
//    Ellipse_CenterAxes,
//    Ellipse_Foci_Bersenham,
//    Ellipse_CenterAxes_Bersenham,
//    Arc_Circle,
//    Arc_Ellipse,
//    Fill
//};
//class DrawBoard : public Renderer
//{
//public:
//    DrawBoard();
//    ~DrawBoard() = default;
//    void OnMouseEvent(int action, float x, float y);
//    void Render(const Scene& scene, const Camera& camera);
//    std::shared_ptr<Walnut::Image> GetFinalImage() const
//    {
//        return m_FinalImage;
//    }
//
//    void AddPrimitive(std::shared_ptr<Primitive> primitive);
//    void SetTemporaryPrimitive(std::shared_ptr<Primitive> primitive);
//    void ClearTemporaryPrimitive();
//    void ClearPrimitives();
//
//    float GetLineWidth() const { return m_lineWidth; }
//    uint32_t GetLineColor() const { return m_lineColor; }
//    uint32_t GetFillColor() const { return m_fillColor; }
//    uint32_t* DrawBoard::GetRenderedImageData() const
//    {
//        uint32_t* m_ImageData_cp(
//            (uint32_t*) malloc(m_FinalImage->GetWidth() *
//                               m_FinalImage->GetHeight() * sizeof(uint32_t)));
//        std::memcpy(m_ImageData_cp, m_ImageData,
//                    m_FinalImage->GetWidth() * m_FinalImage->GetHeight() *
//                        sizeof(uint32_t));
//        return m_ImageData_cp;
//    }
//    uint32_t GetRenderedWidth() const { return m_RenderedWidth; }
//    uint32_t GetRenderedHeight() const { return m_RenderedHeight; }
//
//    void SetDrawingMode(DrawingMode mode) { m_CurrentDrawingMode = mode; }
//    void SetLineWidth(float width) { m_lineWidth = width; }
//    void SetLineColor(uint32_t color) { m_lineColor = color; }
//    void SetFillColor(uint32_t color) { m_fillColor = color; }
//    void SetRenderedWidthAndHeight(uint32_t width, uint32_t height)
//    {
//        m_RenderedWidth = width;
//        m_RenderedHeight = height;
//    }
//
//private:
//    std::vector<std::shared_ptr<Primitive>> m_Primitives;
//    std::shared_ptr<Primitive> m_TemporaryPrimitive;
//    std::vector<glm::vec2> m_CurrentPoints;
//    glm::vec2 currentPoint = {0, 0};
//    glm::vec2 tempPoint = {0, 0};
//    DrawingMode m_CurrentDrawingMode = DrawingMode::None;
//    bool m_IsDrawing = false;
//    bool m_IsHold = false;
//    bool m_IsEmptyPrimitiveRendered = false;
//
//    float m_lineWidth = 0.1f;
//    uint32_t m_lineColor = 0xFFFFFFFF;
//    uint32_t m_fillColor = 0xFFFFFFFF;
//    uint32_t m_RenderedWidth = 0;
//    uint32_t m_RenderedHeight = 0;
//
//    std::shared_ptr<Primitive>
//    CreatePrimitiveFromPoints(const std::vector<glm::vec2>& points);
//    std::shared_ptr<Primitive>
//    CreateTemporaryPrimitive(const std::vector<glm::vec2>& points);
//    size_t DrawBoard::RequiredPointsForCurrentShape() const;
//};
//}  // namespace RMC