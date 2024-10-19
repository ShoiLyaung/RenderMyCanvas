// Algorithms/CohenSutherlandClip.cpp
#include "CohenSutherlandClip.h"

namespace RMC
{

namespace CohenSutherlandClip
{

// 定义区域代码
const int INSIDE = 0;  // 0000
const int LEFT = 1;    // 0001
const int RIGHT = 2;   // 0010
const int BOTTOM = 4;  // 0100
const int TOP = 8;     // 1000

int ComputeOutCode(float x, float y, float xmin, float xmax, float ymin,
                   float ymax)
{
    int code = INSIDE;

    if (x < xmin)
        code |= LEFT;
    else if (x > xmax)
        code |= RIGHT;

    if (y < ymin)
        code |= BOTTOM;
    else if (y > ymax)
        code |= TOP;

    return code;
}

bool ClipLine(const glm::vec2& p0, const glm::vec2& p1, float xmin, float xmax,
              float ymin, float ymax,
              std::vector<std::pair<glm::vec2, glm::vec2>>& clippedLines)
{
    float x0 = p0.x, y0 = p0.y;
    float x1 = p1.x, y1 = p1.y;

    int outcode0 = ComputeOutCode(x0, y0, xmin, xmax, ymin, ymax);
    int outcode1 = ComputeOutCode(x1, y1, xmin, xmax, ymin, ymax);

    bool accept = false;

    while (true) {
        if (!(outcode0 | outcode1)) {
            // 两个端点都在窗口内
            accept = true;
            break;
        } else if (outcode0 & outcode1) {
            // 两个端点在窗口外的同一区域
            break;
        } else {
            // 需要裁剪
            float x, y;
            int outcodeOut = outcode0 ? outcode0 : outcode1;

            if (outcodeOut & TOP) {
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if (outcodeOut & BOTTOM) {
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if (outcodeOut & RIGHT) {
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else if (outcodeOut & LEFT) {
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            if (outcodeOut == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = ComputeOutCode(x0, y0, xmin, xmax, ymin, ymax);
            } else {
                x1 = x;
                y1 = y;
                outcode1 = ComputeOutCode(x1, y1, xmin, xmax, ymin, ymax);
            }
        }
    }

    if (accept) {
        clippedLines.push_back({glm::vec2(x0, y0), glm::vec2(x1, y1)});
    }

    return accept;
}

}  // namespace CohenSutherlandClip

}  // namespace RMC
