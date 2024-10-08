#pragma once
#include "Primitive.h"

class Circle : public Primitive2D
{
public:
    Circle(const glm::vec2& center, float radius);
    virtual void Draw() const override;
private:
    float m_Radius;
};