#pragma once
#include "Primitive.h"

class Line : public Primitive2D
{
public:
    Line(const glm::vec2& start, const glm::vec2& end);
    virtual void Draw() const override;
private:
    glm::vec2 m_Start;
    glm::vec2 m_End;
};