#pragma once
#include "pch.hpp"

namespace RMC::algorithm {
    /**
     * @brief Algorithm for clipping a polygon with respect to a closed boundary.
    */
    class SutherHodgman
    {
    public:
        using int64_t = long long;
        using uint64_t = unsigned long long;
        using Point = std::array<float, 2>;

    public:
        /**
         * @brief Clip a polygon with respect to a closed boundary.
         *
         * @param polygonPoints - Points of the polygon.
         * @param boundary - Points of the boundary.
        */
        void Clip(std::vector<Point>& polygonPoints, const std::vector<Point> boundaryPoints)
        {
            for (uint64_t i = 0; i < boundaryPoints.size(); ++i) {
                int64_t k = (int64_t(i) + 1LL) % int64_t(boundaryPoints.size());
                ClipWithLine(polygonPoints, { boundaryPoints[i], boundaryPoints[k] });
            }
        }

        /**
         * @brief Clip a polygon with respect to a closed boundary.
         *
         * @param polygonPoints - Points of the polygon
         * @param boundary - Points of the boundary
         * @return Clipped polygon points
        */
        std::vector<Point> Clip(const std::vector<Point>& polygonPoints, const std::vector<Point> boundary)
        {
            std::vector<Point> clippedPolygonPoints{ polygonPoints };
            for (uint64_t i = 0; i < boundary.size(); ++i) {
                int64_t k = (int64_t(i) + 1LL) % int64_t(boundary.size());
                ClipWithLine(clippedPolygonPoints, { boundary[i], boundary[k] });
            }
            return clippedPolygonPoints;
        }

    private:
        /**
         * @brief Calculate the intersection point of two lines.
         * @param line0 - Start and end point: {{x0, y0}, {x1, y1}}
         * @param line1 - Start and end point: {{x2, y2}, {x3, y3}}
         * @return The intersection point: {x, y}
        */
        Point Intersect(const std::array<Point, 2>& line0, const std::array<Point, 2>& line1)
        {
            auto& [x0, y0] = line0[0];
            auto& [x1, y1] = line0[1];
            auto& [x2, y2] = line1[0];
            auto& [x3, y3] = line1[1];
            float x = (x0 * y1 - y0 * x1) * (x2 - x3) - (x0 - x1) * (x2 * y3 - y2 * x3);
            float y = (x0 * y1 - y0 * x1) * (y2 - y3) - (y0 - y1) * (x2 * y3 - y2 * x3);
            float w = (x0 - x1) * (y2 - y3) - (y0 - y1) * (x2 - x3);
            return { x / w, y / w };
        }

        /**
         * @brief Clip a polygon with respect to a line.
         *
         * @param polygonPoints - Points of the polygon
         * @param clipLine - Start and end point: {{x0, y0}, {x1, y1}}
        */
        void ClipWithLine(std::vector<Point>& polygonPoints, const std::array<Point, 2>& clipLine)
        {
            std::vector<Point> clippedPolygonPoints{};
            for (uint64_t i = 0; i < polygonPoints.size(); ++i) {
                int64_t k = (int64_t(i) + 1LL) % int64_t(polygonPoints.size());
                Point point_i = polygonPoints[i];
                Point point_k = polygonPoints[k];

                float pos_i = (clipLine[1][0] - clipLine[0][0]) * (point_i[1] - clipLine[0][1])
                    - (clipLine[1][1] - clipLine[0][1]) * (point_i[0] - clipLine[0][0]);

                float pos_k = (clipLine[1][0] - clipLine[0][0]) * (point_k[1] - clipLine[0][1])
                    - (clipLine[1][1] - clipLine[0][1]) * (point_k[0] - clipLine[0][0]);

                // Case 1: When both points are inside
                if (pos_i < 0 && pos_k < 0) {
                    // Only second point is added
                    clippedPolygonPoints.push_back(point_k);
                }
                // Case 2: When only the first point is outside
                else if (pos_i >= 0 && pos_k < 0) {
                    // Point of intersection with edge and the second point is added
                    clippedPolygonPoints.push_back(Intersect({ point_i, point_k }, clipLine));
                    clippedPolygonPoints.push_back(point_k);
                }
                // Case 3: When only the second point is outside
                else if (pos_i < 0 && pos_k >= 0) {
                    // Only point of intersection with edge is added
                    clippedPolygonPoints.push_back(Intersect({ point_i, point_k }, clipLine));
                }
                // Case 4: When both points are outside
                else {
                    // No points are added
                }
            }
            polygonPoints = clippedPolygonPoints;
        }
    };
}
