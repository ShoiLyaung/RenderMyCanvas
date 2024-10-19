#include "ContourExtraction.h"
#include <queue>
#include <unordered_set>

namespace RMC
{

namespace ContourExtraction
{

// 辅助函数，将二维坐标转换为一维索引
inline int Index(int x, int y, int width) { return y * width + x; }

// 定义方向
const std::vector<glm::ivec2> directions = {
    {1, 0},    // 右
    {1, 1},    // 右下
    {0, 1},    // 下
    {-1, 1},   // 左下
    {-1, 0},   // 左
    {-1, -1},  // 左上
    {0, -1},   // 上
    {1, -1}    // 右上
};

bool ExtractContour(uint32_t* imageData, uint32_t width, uint32_t height,
                    const glm::vec3& point, std::vector<glm::vec2>& vertices,
                    uint32_t edgeColor)
{
    int startX = static_cast<int>(point.x + 0.5f);
    int startY = static_cast<int>(point.y + 0.5f);

    if (startX < 0 || startX >= static_cast<int>(width) || startY < 0 ||
        startY >= static_cast<int>(height)) {
        return false;
    }

    uint32_t targetColor = imageData[Index(startX, startY, width)];

    // 如果点击在背景上，直接返回
    if (targetColor == 0) {
        return false;
    }

    // 检查点击点是否在边缘上
    if (targetColor == edgeColor) {
        // 如果在边缘上，稍微偏移一下
        for (const auto& dir : directions) {
            int nx = startX + dir.x;
            int ny = startY + dir.y;
            if (nx >= 0 && nx < static_cast<int>(width) && ny >= 0 &&
                ny < static_cast<int>(height)) {
                uint32_t neighborColor = imageData[Index(nx, ny, width)];
                if (neighborColor != edgeColor && neighborColor != 0) {
                    startX = nx;
                    startY = ny;
                    break;
                }
            }
        }
    }

    // 使用 BFS 或者轮廓跟踪算法提取轮廓
    std::vector<glm::vec2> contour;
    std::unordered_set<int> visited;
    std::queue<glm::ivec2> pixelQueue;

    pixelQueue.push({startX, startY});
    visited.insert(Index(startX, startY, width));

    while (!pixelQueue.empty()) {
        glm::ivec2 current = pixelQueue.front();
        pixelQueue.pop();

        for (const auto& dir : directions) {
            int nx = current.x + dir.x;
            int ny = current.y + dir.y;

            if (nx >= 0 && nx < static_cast<int>(width) && ny >= 0 &&
                ny < static_cast<int>(height)) {
                int idx = Index(nx, ny, width);
                if (visited.count(idx) == 0) {
                    uint32_t color = imageData[idx];
                    if (color == edgeColor) {
                        // 记录边缘点
                        contour.push_back(glm::vec2(nx, ny));
                    } else if (color == targetColor) {
                        // 继续搜索
                        pixelQueue.push({nx, ny});
                    }
                    visited.insert(idx);
                }
            }
        }
    }

    if (contour.empty()) {
        return false;
    }

    // 对轮廓点进行排序或处理，生成顶点列表
    vertices = contour;

    return true;
}

}  // namespace ContourExtraction

}  // namespace RMC
