#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Renderer/Renderer.h"
#include "Primitives/Primitive.h"
#include "Primitives/Line.h"

class DrawBoard : public Renderer
{
public:
    DrawBoard() = default;
	~DrawBoard() = default;
    void OnMouseEvent(int button, int action, int x, int y);
    void Render();
    std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

    void AddPrimitive(std::shared_ptr<Primitive> primitive);
    void SetTemporaryPrimitive(std::shared_ptr<Primitive> primitive);
    void ClearTemporaryPrimitive();
private:
    std::vector<std::shared_ptr<Primitive>> m_Primitives;
    std::shared_ptr<Primitive> m_TemporaryPrimitive;
	glm::vec2 m_TempPoint = glm::vec2(0.0f, 0.0f);
	bool m_IsDrawing = false;
};