#pragma once
#include <glm/glm.hpp>

namespace RMC {

    namespace LineAlgorithm {

        void Bresenham(const glm::vec3& start, const glm::vec3& end,
                   uint32_t* imageData, uint32_t width, uint32_t height,
                   float lineWidth = 0.0f, uint32_t color = 0xFFFFFFFF)
    {
            int x1 = static_cast<int>(start.x);
            int y1 = static_cast<int>(start.y);
            int x2 = static_cast<int>(end.x);
            int y2 = static_cast<int>(end.y);

            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            int sx = (x1 < x2) ? 1 : -1;
            int sy = (y1 < y2) ? 1 : -1;
            int err = dx - dy;

            while (true) {
                for (float dx = -lineWidth; dx <= lineWidth; dx += 0.1f) {
                    for (float dy = -lineWidth; dy <= lineWidth; dy += 0.1f) {
                        int px = static_cast<int>(x1 + dx);
                        int py = static_cast<int>(y1 + dy);

                        if (px >= 0 && px < static_cast<int>(width) &&
                            py >= 0 && py < static_cast<int>(height)) {
                            imageData[py * width + px] = color;
                        }
                    }
                }

                if (x1 == x2 && y1 == y2) break;
                int e2 = 2 * err;
                if (e2 > -dy) {
                    err -= dy;
                    x1 += sx;
                }
                if (e2 < dx) {
                    err += dx;
                    y1 += sy;
                }
            }
        }

    } // namespace LineAlgorithm

} // namespace RMC