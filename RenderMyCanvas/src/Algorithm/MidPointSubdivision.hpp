#pragma once
#include "Utils/Utils.hpp" 
#pragma warning(disable : 4305)

namespace RMC::algorithm {
    class MidPointSubdivision
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
         *
         * @note ||Bug|| The line disappears.
        */
        static std::array<Point, 2> LineClip(Point lineStart, Point lineEnd, Point bottomLeft, Point topRight)
        {
            int code0 = Encode(lineStart, bottomLeft, topRight);
            int code1 = Encode(lineEnd, bottomLeft, topRight);
            int code = 0, x = 0, y = 0;
            float epsilon = 1e-1;

            if (code0 == 0 && code1 == 0) {
                return { lineStart, lineEnd };
            }

            if ((code0 & code1) != 0) {
                return { -1,-1 };
            }


            // If p0 is outside the rectangle, find the nearest point to the rectangle starting from p0
            if (code0 != 0) {
                Point p0{ lineStart }, p1{ lineEnd };
                while (Distance(p0, p1) >= epsilon) {
                    Point middle = { (p0[0] + p1[0]) / 2.0F, (p0[1] + p1[1]) / 2.0F };
                    code = Encode(middle, bottomLeft, topRight);
                    if ((code & code0) == 0) {
                        p1 = middle;
                    } else {
                        p0 = middle;
                        code0 = code;
                    }
                } // Now p0 is the nearest point to the rectangle
                lineStart = p0;
            }

            // If p1 is outside the rectangle, find the nearest point to the rectangle starting from p1
            if (code1 != 0) {
                Point p0{ lineStart }, p1{ lineEnd };
                while (Distance(p0, p1) >= epsilon) {
                    Point middle = { (p0[0] + p1[0]) / 2, (p0[1] + p1[1]) / 2 };
                    code = Encode(middle, bottomLeft, topRight);
                    if ((code & code1) == 0) {
                        p0 = middle;
                    } else {
                        p1 = middle;
                        code1 = code;
                    }
                } // Now p1 is the nearest point to the rectangle
                lineEnd = p1;
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
        static int Encode(const Point& point, const Point& bottomLeft, const Point& topRight)
        {
            auto& [x, y] = point;
            auto& [x_min, y_min] = bottomLeft;
            auto& [x_max, y_max] = topRight;
            int code = 0;
            if (x < x_min) code |= LEFT;
            else if (x > x_max) code |= RIGHT;
            if (y < y_min) code |= BOTTOM;
            else if (y > y_max) code |= TOP;
            return code;
        }

        static float Distance(const Point& point0, const Point poin1)
        {
            return ::sqrt(
                (point0[0] - poin1[0]) * (point0[0] - poin1[0]) + (point0[1] - poin1[1]) * (point0[1] - poin1[1])
            );
        }
    };


}
