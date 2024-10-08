#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <vector>

// Base class for all primitives
class Primitive
{
public:
    virtual ~Primitive() = default;
    virtual void Draw() const = 0; // Pure virtual function to draw the primitive
};

// 2D Primitive derived class
class Primitive2D : public Primitive
{
public:
    Primitive2D(const glm::vec2& position) : m_Position(position) {}
    virtual ~Primitive2D() = default;
protected:
    glm::vec2 m_Position;
};

// 3D Primitive derived class
class Primitive3D : public Primitive
{
public:
    Primitive3D(const glm::vec3& position) : m_Position(position) {}
    virtual ~Primitive3D() = default;
protected:
    glm::vec3 m_Position;
};

// Primitive Factory for creating primitives
class PrimitiveFactory
{
public:
    static std::shared_ptr<Primitive> CreateLine(const glm::vec2& start, const glm::vec2& end);
    static std::shared_ptr<Primitive> CreateCircle(const glm::vec2& center, float radius);
    // Add more creation functions as needed for other primitives
};