// Algorithms/EllipseAlgorithm.h
#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <cmath>

namespace RMC {

    namespace EllipseAlgorithm {

        void Midpoint(const glm::vec3& center, float a, float b, float rotationAngle, uint32_t* imageData, uint32_t width, uint32_t height) {
            // Precompute sine and cosine of rotation angle
            float cosTheta = std::cos(rotationAngle);
            float sinTheta = std::sin(rotationAngle);

            // Square of semi-major and semi-minor axes
            float a2 = a * a;
            float b2 = b * b;

            // Initial positions
            float x = 0;
            float y = b;

            // Decision parameters
            float d1 = b2 - (a2 * b) + (0.25f * a2);
            float dx = 2 * b2 * x;
            float dy = 2 * a2 * y;

            // Region 1
            while (dx < dy) {
                // Plot points in all four quadrants after rotation
                for (int i = -1; i <= 1; i += 2) {
                    for (int j = -1; j <= 1; j += 2) {
                        // Apply rotation
                        float xr = i * x * cosTheta - j * y * sinTheta;
                        float yr = i * x * sinTheta + j * y * cosTheta;

                        int xi = static_cast<int>(xr + center.x + 0.5f);
                        int yi = static_cast<int>(yr + center.y + 0.5f);

                        // Check bounds and plot
                        if (xi >= 0 && xi < static_cast<int>(width) && yi >= 0 && yi < static_cast<int>(height)) {
                            imageData[yi * width + xi] = 0xFFFFFFFF; // White color
                        }
                    }
                }

                if (d1 < 0) {
                    x += 1;
                    dx += 2 * b2;
                    d1 += dx + b2;
                }
                else {
                    x += 1;
                    y -= 1;
                    dx += 2 * b2;
                    dy -= 2 * a2;
                    d1 += dx - dy + b2;
                }
            }

            // Decision parameter for region 2
            float d2 = b2 * (x + 0.5f) * (x + 0.5f) + a2 * (y - 1) * (y - 1) - a2 * b2;

            // Region 2
            while (y >= 0) {
                // Plot points in all four quadrants after rotation
                for (int i = -1; i <= 1; i += 2) {
                    for (int j = -1; j <= 1; j += 2) {
                        // Apply rotation
                        float xr = i * x * cosTheta - j * y * sinTheta;
                        float yr = i * x * sinTheta + j * y * cosTheta;

                        int xi = static_cast<int>(xr + center.x + 0.5f);
                        int yi = static_cast<int>(yr + center.y + 0.5f);

                        // Check bounds and plot
                        if (xi >= 0 && xi < static_cast<int>(width) && yi >= 0 && yi < static_cast<int>(height)) {
                            imageData[yi * width + xi] = 0xFFFFFFFF; // White color
                        }
                    }
                }

                if (d2 > 0) {
                    y -= 1;
                    dy -= 2 * a2;
                    d2 += a2 - dy;
                }
                else {
                    x += 1;
                    y -= 1;
                    dx += 2 * b2;
                    dy -= 2 * a2;
                    d2 += dx - dy + a2;
                }
            }
        }

    } // namespace EllipseAlgorithm

} // namespace RMC
