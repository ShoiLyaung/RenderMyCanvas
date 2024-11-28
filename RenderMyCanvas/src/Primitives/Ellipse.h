// Ellipse.h
#pragma once

#include <glm/glm.hpp>
#include "Primitive.h"

namespace RMC {

    class Ellipse : public Primitive2D {
    public:
        Ellipse(const glm::vec3& center, float majorAxisLength,
                float minorAxisLength, float rotationAngle = 0.0f,
                float lineWidth=0, uint32_t color=0xFFFFFFFF);

        Ellipse(const glm::vec3& focus1, const glm::vec3& focus2,
                float majorAxisLength, float lineWidth = 0,
                uint32_t color = 0xFFFFFFFF);

        void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const override;

    protected:
        glm::vec3 Center;
        float MajorAxisLength;
        float MinorAxisLength;
        glm::vec3 Focus1;
        glm::vec3 Focus2;
        float RotationAngle;
        float LineWidth;
        uint32_t Color;
    };
    class Ellipse_Bresenham : public Ellipse {
    public:
        Ellipse_Bresenham(const glm::vec3& center, float majorAxisLength,
                          float minorAxisLength, float rotationAngle = 0.0f,
                          float lineWidth = 0, uint32_t color = 0xFFFFFFFF);
        Ellipse_Bresenham(const glm::vec3& focus1, const glm::vec3& focus2,
                          float majorAxisLength, float lineWidth = 0,
                          uint32_t color = 0xFFFFFFFF);
        void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const;
	};
    }

