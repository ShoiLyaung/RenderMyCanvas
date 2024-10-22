#pragma once

#include <cmath>
#include <cstdint>
#include <glm/glm.hpp>

namespace RMC
{

namespace EllipseAlgorithm
{

void Midpoint(const glm::vec3& center, double a, double b, double rotationAngle,
              uint32_t* imageData, uint32_t width, uint32_t height,
              float lineWidth = 0.0f, uint32_t color = 0xFFFFFFFF)
{
    // Precompute sine and cosine of rotation angle
    double cosTheta = std::cos(rotationAngle);
    double sinTheta = std::sin(rotationAngle);

    // Square of semi-major and semi-minor axes
    double a2 = a * a;
    double b2 = b * b;

    // Initial positions
    double x = 0;
    double y = b;

    // Decision parameters
    double d1 = b2 - (a2 * b) + (0.25f * a2);
    double dx = 2 * b2 * x;
    double dy = 2 * a2 * y;

    // Region 1
    while (dx < dy) {
        // Plot points in all four quadrants after rotation
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                // Apply rotation
                double xr = i * x * cosTheta - j * y * sinTheta;
                double yr = i * x * sinTheta + j * y * cosTheta;

                int xi = static_cast<int>(xr + center.x + 0.5f);
                int yi = static_cast<int>(yr + center.y + 0.5f);

                for (float dx = -lineWidth; dx <= lineWidth; dx += 0.1f) {
                    for (float dy = -lineWidth; dy <= lineWidth; dy += 0.1f) {
                        int px = static_cast<int>(xi + dx);
                        int py = static_cast<int>(yi + dy);

                        if (px >= 0 && px < static_cast<int>(width) &&
                            py >= 0 && py < static_cast<int>(height)) {
                            imageData[py * width + px] = color;
                        }
                    }
                }
            }
        }

        if (d1 < 0) {
            x += 1;
            dx += 2 * b2;
            d1 += dx + b2;
        } else {
            x += 1;
            y -= 1;
            dx += 2 * b2;
            dy -= 2 * a2;
            d1 += dx - dy + b2;
        }
    }

    // Decision parameter for region 2
    double d2 = b2 * (x + 0.5f) * (x + 0.5f) + a2 * (y - 1) * (y - 1) - a2 * b2;

    // Region 2
    while (y >= 0) {
        // Plot points in all four quadrants after rotation
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                // Apply rotation
                double xr = i * x * cosTheta - j * y * sinTheta;
                double yr = i * x * sinTheta + j * y * cosTheta;

                int xi = static_cast<int>(xr + center.x + 0.5f);
                int yi = static_cast<int>(yr + center.y + 0.5f);

                for (float dx = -lineWidth; dx <= lineWidth; dx += 0.1f) {
                    for (float dy = -lineWidth; dy <= lineWidth; dy += 0.1f) {
                        int px = static_cast<int>(xi + dx);
                        int py = static_cast<int>(yi + dy);

                        if (px >= 0 && px < static_cast<int>(width) &&
                            py >= 0 && py < static_cast<int>(height)) {
                            imageData[py * width + px] = color;
                        }
                    }
                }
            }
        }

        if (d2 > 0) {
            y -= 1;
            dy -= 2 * a2;
            d2 += a2 - dy;
        } else {
            x += 1;
            y -= 1;
            dx += 2 * b2;
            dy -= 2 * a2;
            d2 += dx - dy + a2;
        }
    }
}
void Bresenham(const glm::vec3& center, double a, double b,
               double rotationAngle, uint32_t* imageData, uint32_t width,
               uint32_t height, float lineWidth = 0.0f,
               uint32_t color = 0xFFFFFFFF)
{
    double cosTheta = std::cos(rotationAngle);
    double sinTheta = std::sin(rotationAngle);

    int x0 = static_cast<int>(center.x + 0.5f);
    int y0 = static_cast<int>(center.y + 0.5f);

    double a2 = a * a;
    double b2 = b * b;

    int x = 0;
    int y = static_cast<int>(b + 0.5f);
    double err = b2 - (2 * b - 1) * a2;

    while (b2 * x <= a2 * y) {
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                double xr = i * x * cosTheta - j * y * sinTheta;
                double yr = i * x * sinTheta + j * y * cosTheta;

                int xi = static_cast<int>(xr + x0 + 0.5f);
                int yi = static_cast<int>(yr + y0 + 0.5f);

                for (float dx = -lineWidth; dx <= lineWidth; dx += 0.1f) {
                    for (float dy = -lineWidth; dy <= lineWidth; dy += 0.1f) {
                        int px = static_cast<int>(xi + dx);
                        int py = static_cast<int>(yi + dy);

                        if (px >= 0 && px < static_cast<int>(width) &&
                            py >= 0 && py < static_cast<int>(height)) {
                            imageData[py * width + px] = color;
                        }
                    }
                }
            }
        }

        if (err > 0) {
            err -= 2 * a2 * y--;
        }
        err += 2 * b2 * ++x;
    }

    x = static_cast<int>(a + 0.5f);
    y = 0;
    err = a2 - (2 * a - 1) * b2;

    while (a2 * y <= b2 * x) {
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                double xr = i * x * cosTheta - j * y * sinTheta;
                double yr = i * x * sinTheta + j * y * cosTheta;

                int xi = static_cast<int>(xr + x0 + 0.5f);
                int yi = static_cast<int>(yr + y0 + 0.5f);

                for (float dx = -lineWidth; dx <= lineWidth; dx += 0.1f) {
                    for (float dy = -lineWidth; dy <= lineWidth; dy += 0.1f) {
                        int px = static_cast<int>(xi + dx);
                        int py = static_cast<int>(yi + dy);

                        if (px >= 0 && px < static_cast<int>(width) &&
                            py >= 0 && py < static_cast<int>(height)) {
                            imageData[py * width + px] = color;
                        }
                    }
                }
            }
        }

        if (err > 0) {
            err -= 2 * b2 * x--;
        }
        err += 2 * a2 * ++y;
    }
}

}  // namespace EllipseAlgorithm

}  // namespace RMC
