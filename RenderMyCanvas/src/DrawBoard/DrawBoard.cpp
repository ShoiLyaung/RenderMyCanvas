#include "DrawBoard.h"
#include <iostream>

namespace RMC
{
DrawBoard::DrawBoard()
{
    m_uv0 = ImVec2(0, 0);
    m_uv1 = ImVec2(1, 1);
}

void DrawBoard::OnMouseEvent(int action, float x, float y)
{
    glm::vec2 currentPos(x, y);

    if (m_CurrentDrawingMode == DrawingMode::Fill) {
        if (action == 1) {
            std::vector<glm::vec2> vertices;
            if (RMC::ContourExtraction::ExtractContour(
                    m_ImageData, m_FinalImage->GetWidth(),
                    m_FinalImage->GetHeight(), glm::vec3(x, y, 0.0f),
                    vertices)) {
                std::cout<<"PolygonScanlineFill\n";
                RMC::PolygonScanlineFill::FillPolygon(
                    vertices, m_ImageData, m_FinalImage->GetWidth(),
                    m_FinalImage->GetHeight(), m_fillColor);
            } else {
                std::cout << "SeedFill\n";
                RMC::SeedFill::FillArea(m_ImageData, m_FinalImage->GetWidth(),
                                        m_FinalImage->GetHeight(), m_fillColor,
                                        glm::vec3(x, y, 0.0f));
            }

            m_FinalImage->SetData(m_ImageData);
        }
    } else if (action == 1 && m_IsDrawing == false) {
        m_IsDrawing = true;
        m_CurrentPoints.push_back(currentPos);
        if (m_CurrentPoints.size() >= RequiredPointsForCurrentShape()) {
            ClearTemporaryPrimitive();
            AddPrimitive(CreatePrimitiveFromPoints(m_CurrentPoints));
            m_CurrentPoints.clear();
        }
    } else if (action == 0) {
        m_IsDrawing = false;
        m_CurrentPoints.push_back(currentPos);
        if (m_CurrentPoints.size() >= RequiredPointsForCurrentShape()) {
            SetTemporaryPrimitive(CreateTemporaryPrimitive(m_CurrentPoints));
        }
        m_CurrentPoints.pop_back();
    }
}

void DrawBoard::Render(const Scene& scene, const Camera& camera)
{
    if (!m_IsEmptyPrimitiveRendered) {
        if (m_ImageData != nullptr) {
            free(m_ImageData);
            m_ImageData = nullptr;
        }
        m_ImageData =
            (uint32_t*) malloc(m_FinalImage->GetWidth() *
                               m_FinalImage->GetHeight() * sizeof(uint32_t));
        std::memset(m_ImageData, 0,
                    m_FinalImage->GetWidth() * m_FinalImage->GetHeight() *
                        sizeof(uint32_t));
        SetRenderedWidthAndHeight(m_FinalImage->GetWidth(),
                                  m_FinalImage->GetHeight());
        m_IsEmptyPrimitiveRendered = true;
    } else if (GetRenderedHeight() != m_FinalImage->GetHeight() ||
               GetRenderedWidth() != m_FinalImage->GetWidth()) {
        for (auto& primitive : m_Primitives) {
            if (primitive)
                primitive->SetRendered(false);
        }
        if (m_ImageData != nullptr) {
			free(m_ImageData);
			m_ImageData = nullptr;
		}
        m_ImageData =
            (uint32_t*) malloc(m_FinalImage->GetWidth() *
                               m_FinalImage->GetHeight() * sizeof(uint32_t));
        std::memset(m_ImageData, 0,
                    m_FinalImage->GetWidth() * m_FinalImage->GetHeight() *
                        sizeof(uint32_t));
        SetRenderedWidthAndHeight(m_FinalImage->GetWidth(),
                                  m_FinalImage->GetHeight());
    } else {
        for (auto primitive : m_Primitives) {
            if (primitive)
                if (!primitive->IsRendered()) {
                    primitive->Draw(m_ImageData, m_FinalImage->GetWidth(),
                                    m_FinalImage->GetHeight());
                    primitive->Draw(m_ImageData, m_FinalImage->GetWidth(),
                                    m_FinalImage->GetHeight());
                    primitive->SetRendered(true);
                }
        }

        auto m_ImageData_t = GetRenderedImageData();
        if (m_TemporaryPrimitive) {

            m_TemporaryPrimitive->Draw(m_ImageData_t, m_FinalImage->GetWidth(),
                                       m_FinalImage->GetHeight());
        }
        m_FinalImage->SetData(m_ImageData_t);
        if (m_ImageData_t != nullptr) {
            free(m_ImageData_t);
            m_ImageData_t = nullptr;
        }
    }
}

void DrawBoard::AddPrimitive(std::shared_ptr<Primitive> primitive)
{
    m_Primitives.push_back(primitive);
}

void DrawBoard::SetTemporaryPrimitive(std::shared_ptr<Primitive> primitive)
{
    m_TemporaryPrimitive = primitive;
}

void DrawBoard::ClearTemporaryPrimitive() { m_TemporaryPrimitive.reset(); }

std::shared_ptr<Primitive>
DrawBoard::CreatePrimitiveFromPoints(const std::vector<glm::vec2>& points)
{
    switch (m_CurrentDrawingMode) {
    case DrawingMode::Line:
        return std::make_shared<Line>(glm::vec3(points[0], 0.0f),
                                      glm::vec3(points[1], 0.0f), m_lineWidth,
                                      m_lineColor);
    case DrawingMode::Circle_CenterRadius:
        return std::make_shared<Circle>(glm::vec3(points[0], 0.0f),
                                        glm::distance(points[0], points[1]),
                                        m_lineWidth, m_lineColor);
    case DrawingMode::Circle_Diameter:
        return std::make_shared<Circle>(glm::vec3(points[0], 0.0f),
                                        glm::vec3(points[1], 0.0f), m_lineWidth,
                                        m_lineColor);
    case DrawingMode::Ellipse_Foci: {
        glm::vec3 focus1(points[0], 0.0f);
        glm::vec3 focus2(points[1], 0.0f);
        glm::vec3 pointOnEllipse(points[2], 0.0f);

        double distance1 = glm::distance(pointOnEllipse, focus1);
        double distance2 = glm::distance(pointOnEllipse, focus2);
        double majorAxisLength = (distance1 + distance2) * 0.5f;

        return std::make_shared<Ellipse>(focus1, focus2, majorAxisLength,
                                         m_lineWidth, m_lineColor);
    }
    case DrawingMode::Ellipse_CenterAxes: {
        glm::vec3 center(points[0], 0.0f);
        glm::vec3 pointOnMajorAxis(points[1], 0.0f);
        glm::vec3 pointOnMinorAxis(points[2], 0.0f);

        double majorAxisLength = glm::distance(center, pointOnMajorAxis);
        double minorAxisLength = glm::distance(center, pointOnMinorAxis);
        double rotationAngle = std::atan2(pointOnMajorAxis.y - center.y,
                                          pointOnMajorAxis.x - center.x);

        return std::make_shared<Ellipse>(center, majorAxisLength,
                                         minorAxisLength, rotationAngle,
                                         m_lineWidth, m_lineColor);
    }
    case DrawingMode::Ellipse_Foci_Bersenham: {
        glm::vec3 focus1(points[0], 0.0f);
        glm::vec3 focus2(points[1], 0.0f);
        glm::vec3 pointOnEllipse(points[2], 0.0f);

        double distance1 = glm::distance(pointOnEllipse, focus1);
        double distance2 = glm::distance(pointOnEllipse, focus2);
        double majorAxisLength = (distance1 + distance2) * 0.5f;

        return std::make_shared<Ellipse_Bresenham>(
            focus1, focus2, majorAxisLength, m_lineWidth, m_lineColor);
    }
    case DrawingMode::Ellipse_CenterAxes_Bersenham: {
        glm::vec3 center(points[0], 0.0f);
        glm::vec3 pointOnMajorAxis(points[1], 0.0f);
        glm::vec3 pointOnMinorAxis(points[2], 0.0f);

        double majorAxisLength = glm::distance(center, pointOnMajorAxis);
        double minorAxisLength = glm::distance(center, pointOnMinorAxis);

        double rotationAngle = std::atan2(pointOnMajorAxis.y - center.y,
                                          pointOnMajorAxis.x - center.x);

        return std::make_shared<Ellipse_Bresenham>(
            center, majorAxisLength, minorAxisLength, rotationAngle,
            m_lineWidth, m_lineColor);
    }
    case DrawingMode::Arc_Circle: {
        glm::vec3 center(points[0], 0.0f);
        glm::vec3 startPoint(points[1], 0.0f);
        glm::vec3 endPoint(points[2], 0.0f);

        double radius = glm::distance(center, startPoint);

        double startAngle =
            std::atan2(startPoint.y - center.y, startPoint.x - center.x);
        double endAngle =
            std::atan2(endPoint.y - center.y, endPoint.x - center.x);

        return std::make_shared<Arc>(center, radius, startAngle, endAngle,
                                     m_lineWidth, m_lineColor);
    }
    case DrawingMode::Arc_Ellipse: {
        glm::vec3 focus1(points[0], 0.0f);
        glm::vec3 focus2(points[1], 0.0f);
        glm::vec3 startPoint(points[2], 0.0f);
        glm::vec3 endPoint(points[3], 0.0f);

        double focalDistance = glm::distance(focus1, focus2);
        double startToFocal1 = glm::distance(startPoint, focus1);
        double startToFocal2 = glm::distance(startPoint, focus2);
        double majorAxisLength = (startToFocal1 + startToFocal2) / 2.0f;
        double minorAxisLength =
            std::sqrt(majorAxisLength * majorAxisLength -
                      focalDistance * focalDistance / 4.0f);

        glm::vec3 center = (focus1 + focus2) / 2.0f;

        double rotationAngle =
            std::atan2(focus2.y - focus1.y, focus2.x - focus1.x);

        double cosTheta = std::cos(-rotationAngle);
        double sinTheta = std::sin(-rotationAngle);

        glm::vec2 relativeStart = startPoint - center;
        glm::vec2 relativeEnd = endPoint - center;

        double xr = relativeStart.x * cosTheta - relativeStart.y * sinTheta;
        double yr = relativeStart.x * sinTheta + relativeStart.y * cosTheta;
        double startAngle =
            std::atan2(yr / majorAxisLength, xr / majorAxisLength);

        xr = relativeEnd.x * cosTheta - relativeEnd.y * sinTheta;
        yr = relativeEnd.x * sinTheta + relativeEnd.y * cosTheta;
        double endAngle =
            std::atan2(yr / majorAxisLength, xr / majorAxisLength);

        return std::make_shared<Arc>(center, majorAxisLength, minorAxisLength,
                                     rotationAngle, startAngle, endAngle,
                                     m_lineWidth, m_lineColor);
    }

    default:
        return nullptr;
    }
}

std::shared_ptr<Primitive>
DrawBoard::CreateTemporaryPrimitive(const std::vector<glm::vec2>& points)
{
    return CreatePrimitiveFromPoints(points);
}

size_t DrawBoard::RequiredPointsForCurrentShape() const
{
    switch (m_CurrentDrawingMode) {
    case DrawingMode::Line:
        return 2;
    case DrawingMode::Circle_CenterRadius:
        return 2;
    case DrawingMode::Circle_Diameter:
        return 2;
    case DrawingMode::Ellipse_Foci:
        return 3;
    case DrawingMode::Ellipse_CenterAxes:
        return 3;
    case DrawingMode::Ellipse_CenterAxes_Bersenham:
        return 3;
    case DrawingMode::Ellipse_Foci_Bersenham:
        return 3;
    case DrawingMode::Arc_Circle:
        return 3;
    case DrawingMode::Arc_Ellipse:
        return 4;
    case DrawingMode::Fill:
        return 1;
    default:
        return 10000;
    }
}
}  // namespace RMC