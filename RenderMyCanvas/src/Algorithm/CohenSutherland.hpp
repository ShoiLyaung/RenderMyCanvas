#pragma once
#include "AlgoPredefines.hpp"
#pragma warning(disable: 4244)

namespace RMC::algorithm {
    class CohenSutherland
    {
    public:
        using Point = std::array<float, 2>;

    public:
        /**
         * @brief Clip a line segment with respect to a rectangle.
         *
         * @param lineStart - Start point of the line segment: {x1, y1}.
         * @param lineEnd - End point of the line segment: {x2, y2}.
         * @param bottomLeft - Bottom left point of the rectangle: {x_min, y_min}.
         * @param topRight - Top right point of the rectangle: {x_max, y_max}.
         * @return If the line segment is completely outside the rectangle, return {-1, -1}; 
         *         Otherwise, return the clipped line segment: {{x1, y1}, {x2, y2}}.
        */
        static auto LineClip(Point lineStart, Point lineEnd, Point bottomLeft, Point topRight) -> std::array<Point, 2>
        {
            int code0 = Encode(lineStart, bottomLeft, topRight);
            int code1 = Encode(lineEnd, bottomLeft, topRight);
            int code = 0, x = 0, y = 0;

            // Loop until both points are inside the rectangle
            while (code0 != 0 || code1 != 0) {
                // If both points are outside rectangle, in same region, reject line
                if ((code0 & code1) != 0) {
                    return { -1, -1 };
                }
                // Pick a point outside rectangle
                code = code0 == 0 ? code1: code0;
                // If intersection point is on the left 
                if ((LEFT & code) != 0) {
                    x = bottomLeft[0];
                    y = lineStart[1] + (lineEnd[1] - lineStart[1]) * (x - lineStart[0]) / (lineEnd[0] - lineStart[0]);
                } 
                // Or if intersection point is on the right 
                else if ((RIGHT & code) != 0) {
                    x = topRight[0];
                    y = lineStart[1] + (lineEnd[1] - lineStart[1]) * (x - lineStart[0]) / (lineEnd[0] - lineStart[0]);
                } 
                // Or if intersection point is on the bottom 
                else if ((BOTTOM & code) != 0) {
                    y = bottomLeft[1];
                    x = lineStart[0] + (lineEnd[0] - lineStart[0]) * (y - lineStart[1]) / (lineEnd[1] - lineStart[1]);
                }
                // Or if intersection point is on the top
                else if ((TOP & code) != 0) {
                    y = topRight[1];
                    x = lineStart[0] + (lineEnd[0] - lineStart[0]) * (y - lineStart[1]) / (lineEnd[1] - lineStart[1]);
                }

                // Clip the line from outside point to intersection point
                if (code == code0) {
                    lineStart[0] = x;
                    lineStart[1] = y;
                    code0 = Encode(lineStart, bottomLeft, topRight);
                } else {
                    lineEnd[0] = x;
                    lineEnd[1] = y;
                    code1 = Encode(lineEnd, bottomLeft, topRight);
                }
            }
            return { lineStart, lineEnd };
        }

    private:
        static const int LEFT = 1;
        static const int RIGHT = 2;
        static const int BOTTOM = 4;
        static const int TOP = 8;

        /**
         * @brief Encode a point with respect to a rectangle.
         *
         * @param point - Point to be encoded: {x, y}.
         * @param bottom_left - Bottom left point of the rectangle: {x_min, y_min}.
         * @param top_right - Top right point of the rectangle: {x_max, y_max}.
         * @return Encoded code.
        */
        static int Encode(Point point, Point bottomLeft, Point topRight)
        {
            auto [x, y] = point;
            auto [x_min, y_min] = bottomLeft;
            auto [x_max, y_max] = topRight;
            int code = 0;
            if (x < x_min) code |= LEFT;
            else if (x > x_max) code |= RIGHT;
            if (y < y_min) code |= BOTTOM;
            else if (y > y_max) code |= TOP;
            return code;
        }
    };
}