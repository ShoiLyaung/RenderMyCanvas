#pragma once
#include <glm/glm.hpp>

namespace RMC {

    namespace CircleAlgorithm {

        void MidPoint(const glm::vec3& center, float radius, uint32_t* imageData,
                  uint32_t width, uint32_t height, float lineWidth,
                  uint32_t color)
    {
            int x0 = static_cast<int>(center.x);
            int y0 = static_cast<int>(center.y);
            int r = static_cast<int>(radius);

            int x = 0;
            int y = r;
            int d = 1 - r;

            auto setPixel = [&](int px, int py) {
                if (px >= 0 && px < static_cast<int>(width) && py >= 0 && py < static_cast<int>(height)) {
                    imageData[py * width + px] = 0xFFFFFFFF;
                }
                for (float dx = -lineWidth; dx <= lineWidth; dx+=0.1f) {
                    for (float dy = -lineWidth; dy <= lineWidth; dy+=0.1f) {
                        int px1 = static_cast<int>(px + dx);
                        int py1 = static_cast<int>(py + dy);

                        if (px1 >= 0 && px1 < static_cast<int>(width) &&
                            py1 >= 0 && py1 < static_cast<int>(height)) {
                            imageData[py1 * width + px1] = color;
                        }
                    }
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

    } // namespace CircleAlgorithm

} // namespace RMC
