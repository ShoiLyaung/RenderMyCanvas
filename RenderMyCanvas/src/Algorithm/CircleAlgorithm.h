#pragma once
#include <glm/glm.hpp>

namespace RMC {

    namespace CircleAlgorithm {

        void MidPoint(const glm::vec3& center, float radius, uint32_t* imageData, uint32_t width, uint32_t height) {
            int x0 = static_cast<int>(center.x);
            int y0 = static_cast<int>(center.y);
            int r = static_cast<int>(radius);

            int x = 0;
            int y = r;
            int d = 1 - r;

            auto setPixel = [&](int px, int py) {
                if (px >= 0 && px < static_cast<int>(width) && py >= 0 && py < static_cast<int>(height)) {
                    imageData[py * width + px] = 0xFFFFFFFF; // Set pixel to white (RGBA)
                }
                };

            // Draw the initial set of points
            setPixel(x0, y0 + r);
            setPixel(x0, y0 - r);
            setPixel(x0 + r, y0);
            setPixel(x0 - r, y0);

            while (x < y) {
                x++;
                if (d < 0) {
                    d += 2 * x + 1;
                }
                else {
                    y--;
                    d += 2 * (x - y) + 1;
                }

                setPixel(x0 + x, y0 + y);
                setPixel(x0 - x, y0 + y);
                setPixel(x0 + x, y0 - y);
                setPixel(x0 - x, y0 - y);
                setPixel(x0 + y, y0 + x);
                setPixel(x0 - y, y0 + x);
                setPixel(x0 + y, y0 - x);
                setPixel(x0 - y, y0 - x);
            }
        }

    } // namespace CircleAlgorithms

} // namespace RMC
