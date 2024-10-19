// Algorithms/SeedFill.cpp
#include "SeedFill.h"
#include <stack>

namespace RMC
{

namespace SeedFill
{

void FillArea(uint32_t* imageData, uint32_t width, uint32_t height,
              uint32_t fillColor, const glm::vec3& point)
{
    // 将 glm::vec3 转换为整数坐标
    int x = static_cast<int>(point.x + 0.5f);
    int y = static_cast<int>(point.y + 0.5f);

    // 检查种子点是否在图像范围内
    if (x < 0 || x >= static_cast<int>(width) || y < 0 ||
        y >= static_cast<int>(height)) {
        return;
    }

    uint32_t targetColor = imageData[y * width + x];
    if (targetColor == fillColor) {
        return;
    }

    std::stack<glm::ivec2> pixelStack;
    pixelStack.push(glm::ivec2(x, y));

    while (!pixelStack.empty()) {
        glm::ivec2 pt = pixelStack.top();
        pixelStack.pop();

        int px = pt.x;
        int py = pt.y;

        // 检查是否越界
        if (px < 0 || px >= static_cast<int>(width) || py < 0 ||
            py >= static_cast<int>(height)) {
            continue;
        }

        uint32_t currentColor = imageData[py * width + px];
        if (currentColor != targetColor) {
            continue;
        }

        imageData[py * width + px] = fillColor;

        // 四连通填充
        pixelStack.push(glm::ivec2(px + 1, py));
        pixelStack.push(glm::ivec2(px - 1, py));
        pixelStack.push(glm::ivec2(px, py + 1));
        pixelStack.push(glm::ivec2(px, py - 1));
    }
}

}  // namespace SeedFill

}  // namespace RMC
