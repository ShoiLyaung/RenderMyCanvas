#pragma once

#include <glm/glm.hpp>
#include "Algorithm/AlgorithmType.h"

namespace RMC
{
    // Base class for all primitives
    class Primitive
    {
    public:
        Primitive() = default;
        ~Primitive() = default;
		virtual void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const = 0;
    protected:
        //AlgorithmType Algorithm;
    };

    class Primitive2D : public Primitive
    {
	public:
		Primitive2D() = default;
		~Primitive2D() = default;
	protected:
		glm::vec4 Color;
		bool isFilled;
	};

    class Primitive3D : public Primitive
    {
	public:
		Primitive3D() = default;
		~Primitive3D() = default;
	protected:
		glm::vec3 Center = glm::vec3(0.0f);
		glm::vec4 Color = glm::vec4(1.0f);
		bool isFilled = 0;
	};

} // namespace RMC