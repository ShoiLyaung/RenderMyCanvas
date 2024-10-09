#pragma once
#include "Primitive.h"

class Circle : public Primitive2D
{
public:
    Circle(const glm::vec2& center, float radius);
    virtual void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const override;
private:
    float m_Radius;
};