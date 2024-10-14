#include "DrawBoard.h"

namespace RMC {
DrawBoard::DrawBoard() {
  m_uv0 = ImVec2(0, 0);
  m_uv1 = ImVec2(1, 1);
}

void DrawBoard::OnMouseEvent(int action, float x, float y) {
  glm::vec2 currentPos(x, y);

  if (action == 1 && m_IsDrawing == false) {
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

void DrawBoard::Render(const Scene &scene, const Camera &camera) {
  std::memset(m_ImageData, 0,
              m_FinalImage->GetWidth() * m_FinalImage->GetHeight() *
                  sizeof(uint32_t));

  for (auto &primitive : m_Primitives) {
    if (primitive)
      primitive->Draw(m_ImageData, m_FinalImage->GetWidth(),
                      m_FinalImage->GetHeight());
  }

  if (m_TemporaryPrimitive)
    m_TemporaryPrimitive->Draw(m_ImageData, m_FinalImage->GetWidth(),
                               m_FinalImage->GetHeight());

  m_FinalImage->SetData(m_ImageData);
}

void DrawBoard::AddPrimitive(std::shared_ptr<Primitive> primitive) {
  m_Primitives.push_back(primitive);
}

void DrawBoard::SetTemporaryPrimitive(std::shared_ptr<Primitive> primitive) {
  m_TemporaryPrimitive = primitive;
}

void DrawBoard::ClearTemporaryPrimitive() { m_TemporaryPrimitive.reset(); }

std::shared_ptr<Primitive> DrawBoard::CreatePrimitiveFromPoints(const std::vector<glm::vec2>& points) {
    switch (m_CurrentDrawingMode) {
    case DrawingMode::Line:
        return std::make_shared<Line>(glm::vec3(points[0], 0.0f), glm::vec3(points[1], 0.0f));
    case DrawingMode::Circle_CenterRadius:
        return std::make_shared<Circle>(glm::vec3(points[0], 0.0f), glm::distance(points[0], points[1]));
    case DrawingMode::Ellipse_Foci: {
        glm::vec3 focus1(points[0], 0.0f);
        glm::vec3 focus2(points[1], 0.0f);
        glm::vec3 pointOnEllipse(points[2], 0.0f);

        float distance1 = glm::distance(pointOnEllipse, focus1);
        float distance2 = glm::distance(pointOnEllipse, focus2);
        float majorAxisLength = (distance1 + distance2) * 0.5f;

        return std::make_shared<Ellipse>(focus1, focus2, majorAxisLength);
    }
    case DrawingMode::Ellipse_CenterAxes: {
        glm::vec3 center(points[0], 0.0f);
        glm::vec3 pointOnMajorAxis(points[1], 0.0f);
        glm::vec3 pointOnMinorAxis(points[2], 0.0f);

        float majorAxisLength = glm::distance(center, pointOnMajorAxis);
        float minorAxisLength = glm::distance(center, pointOnMinorAxis);

        float rotationAngle = std::atan2(pointOnMajorAxis.y - center.y, pointOnMajorAxis.x - center.x);

        return std::make_shared<Ellipse>(center, majorAxisLength, minorAxisLength, rotationAngle);
    }
    default:
        return nullptr;
    }
}

std::shared_ptr<Primitive>
DrawBoard::CreateTemporaryPrimitive(const std::vector<glm::vec2> &points) {
  return CreatePrimitiveFromPoints(points);
}

size_t DrawBoard::RequiredPointsForCurrentShape() const {
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
  default:
    return 10000;
  }
}
} // namespace RMC