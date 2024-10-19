#include "ContourExtraction.h"
#include <queue>
#include <unordered_set>

namespace RMC
{

namespace ContourExtraction
{

// ��������������ά����ת��Ϊһά����
inline int Index(int x, int y, int width) { return y * width + x; }

// ���巽��
const std::vector<glm::ivec2> directions = {
    {1, 0},    // ��
    {1, 1},    // ����
    {0, 1},    // ��
    {-1, 1},   // ����
    {-1, 0},   // ��
    {-1, -1},  // ����
    {0, -1},   // ��
    {1, -1}    // ����
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

    // �������ڱ����ϣ�ֱ�ӷ���
    if (targetColor == 0) {
        return false;
    }

    // ��������Ƿ��ڱ�Ե��
    if (targetColor == edgeColor) {
        // ����ڱ�Ե�ϣ���΢ƫ��һ��
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

    // ʹ�� BFS �������������㷨��ȡ����
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
                        // ��¼��Ե��
                        contour.push_back(glm::vec2(nx, ny));
                    } else if (color == targetColor) {
                        // ��������
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

    // ���������������������ɶ����б�
    vertices = contour;

    return true;
}

}  // namespace ContourExtraction

}  // namespace RMC
