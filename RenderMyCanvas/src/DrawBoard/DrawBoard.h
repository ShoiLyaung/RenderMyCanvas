#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Renderer/Renderer.h"
#include "Primitives/Primitive.h"
#include "Primitives/Line.h"
#include "Primitives/Circle.h"
#include "Primitives/Ellipse.h"
#include "Primitives/Arc.h"

namespace RMC {
    enum class DrawingMode {
        None,
        Line,
        Circle_CenterRadius,
        Circle_Diameter,
        Ellipse_Foci,
        Ellipse_CenterAxes, 
        Ellipse_Foci_Bersenham,
        Ellipse_CenterAxes_Bersenham,
        Arc_Circle,
        Arc_Ellipse,
    };
    class DrawBoard : public Renderer
    {
    public:
        DrawBoard();
        ~DrawBoard() = default;
        void OnMouseEvent(int action, float x, float y);
        void Render(const Scene& scene, const Camera& camera);
        std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

        void AddPrimitive(std::shared_ptr<Primitive> primitive);
        void SetTemporaryPrimitive(std::shared_ptr<Primitive> primitive);
        void ClearTemporaryPrimitive();

        void SetDrawingMode(DrawingMode mode) { m_CurrentDrawingMode = mode; }
    private:
        std::vector<std::shared_ptr<Primitive>> m_Primitives;
        std::shared_ptr<Primitive> m_TemporaryPrimitive;
        std::vector<glm::vec2> m_CurrentPoints;
		glm::vec2 currentPoint = { 0, 0 };
		glm::vec2 tempPoint = { 0, 0 };
        DrawingMode m_CurrentDrawingMode = DrawingMode::None;
        bool m_IsDrawing = false;
		bool m_IsHold = false;

        std::shared_ptr<Primitive> CreatePrimitiveFromPoints(const std::vector<glm::vec2>& points);
        std::shared_ptr<Primitive> CreateTemporaryPrimitive(const std::vector<glm::vec2>& points);
		size_t DrawBoard::RequiredPointsForCurrentShape() const;
    };
} // namespace RMC