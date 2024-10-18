// Algorithms/ArcAlgorithm.h
#pragma once

#include <cmath>
#include <cstdint>
#include <glm/glm.hpp>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

namespace RMC
{

namespace ArcAlgorithm
{

void DrawArc(const glm::vec3& center, double a, double b, double rotationAngle,
             double startAngle, double endAngle, uint32_t* imageData,
             uint32_t width, uint32_t height, float lineWidth = 0.0f,
             uint32_t color = 0xFFFFFFFF)
{
    // int lineWidth = 0;
    const int numSteps = 20000;

    while (startAngle < 0)
        startAngle += 2 * M_PI;
    while (endAngle < 0)
        endAngle += 2 * M_PI;
    if (endAngle < startAngle)
        endAngle += 2 * M_PI;

    double angleRange = endAngle - startAngle;

    double cosTheta = std::cos(rotationAngle);
    double sinTheta = std::sin(rotationAngle);

    for (int i = 0; i <= numSteps; ++i) {
        double progress = static_cast<double>(i) / numSteps;
        // double t = startAngle + (angleRange * i) / numSteps;
        double t = startAngle + angleRange * progress;
        double x = a * std::cos(t);
        double y = b * std::sin(t);

        double xr = x * cosTheta - y * sinTheta;
        double yr = x * sinTheta + y * cosTheta;

        int xi = static_cast<int>(xr + center.x);
        int yi = static_cast<int>(yr + center.y);

        for (float dx = -lineWidth; dx <= lineWidth; dx += 0.1f) {
            for (float dy = -lineWidth; dy <= lineWidth; dy += 0.1f) {
                int px = static_cast<int>(xi + dx);
                int py = static_cast<int>(yi + dy);

                if (px >= 0 && px < static_cast<int>(width) && py >= 0 &&
                    py < static_cast<int>(height)) {
                    imageData[py * width + px] = color;
                }
            }
        }
    }
}

}  // namespace ArcAlgorithm

}  // namespace RMC
