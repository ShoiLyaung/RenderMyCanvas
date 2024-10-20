#pragma once
#include "Utils.hpp"

namespace RMC::utils {
    class CoordTransformer
    {
    public:
        explicit CoordTransformer() = default;

        /**
         * @brief Get 2D translation matrix.
         * @param offset - Dest coord minus origin coord, i.e., [offset_x, offset_y].
         * @return A 3x3 matrix of 2D translation.
        */
        glm::mat3 MatTranslate(const glm::vec2& offset) const;

        /**
         * @brief Get 3D translation matrix.
         * @param offset - Dest coord minus origin coord, i.e., [offset_x, offset_y, offset_z].
         * @return A 4x4 matrix of 2D translation.
        */
        glm::mat4 MatTranslate(const glm::vec3& offset) const;

        /**
         * @brief Get 2D rotattion matrix.
         * @param degree - Rotation degree, clockwise is negative.
         * @return A 3x3 matrix of 2D rotation.
        */
        glm::mat3 MatRotate(float degree) const;

        /**
         * @brief Get 3D rotattion matrix.
         * @param degree - Rotation degree, clockwise is negative.
         * @return A 3x3 matrix of 2D rotation.
        */
        glm::mat4 MatRotate(glm::quat rotation) const;

        /**
         * @brief Get 2D flip matrix.
         * @param dir - 0 for horizontal flip, 1 for vertical flip, 2 for transpose (exchange x and y).
         * @return A 3x3 matrix of 2D flip.
        */
        glm::mat3 MatFlip(int8_t dir) const;

        /**
         * @brief Get 2D scale matrix.
         * @param scale - [scale_x, scale_y].
         * @return A 3x3 matrix of 2D scale.
        */
        glm::mat3 MatScale(const glm::vec2& scale) const;

        /**
         * @brief Get 3D scale matrix.
         * @param scale - [scale_x, scale_y].
         * @return A 4x4 matrix of 3D scale.
        */
        glm::mat4 MatScale(const glm::vec3& scale) const;

        /**
         * @brief Set a transform mat for transforming a 2D point to the desired position.
         * @param mat - A 3x3 matrix for transform.
        */
        void Set2DTransformMatrix(const glm::mat3& mat) { m_transformMatrix = mat; }

        /**
         * @brief Set a transform mat for transforming a 2D point to the desired position.
         * @param mat - A 4x4 matrix for transform.
        */
        void Set3DTransformMatrix(const glm::mat4& mat) { m_3dTransformMatrix = mat; }

        /**
         * @brief Set a transform mat for transforming a 2D point back to its origin position
         * @param mat - A 3x3 matrix for inverse transform.
        */
        void SetInverseTransformMatrix(const glm::mat3& mat) { m_inverseTransformMatrix = mat; }

        template<typename T>
            requires IsPoint2D<T>
        T Transform2D(const T& coord) {
            auto res = glm::vec3(coord[0], coord[1], 1) * m_transformMatrix;
            return { res[0], res[1] };
        }

        template<typename T>
            requires IsPoint3D<T>
        T Transform3D(const T& coord) {
            auto res = glm::vec4(coord[0], coord[1], coord[2], 1) * m_3dTransformMatrix;
            return { res[0], res[1], res[2] };
        }

        template<typename T>
        T InverseTransform(const T& coord) {
            auto res = glm::vec3(coord[0], coord[1], 1.0F) * m_inverseTransformMatrix;
            return { res[0], res[1] };
        }

        template<typename T>
        auto GetTransformFunc() -> std::function<T(const T&)> {
            return [this](const T& coord) { return Transform(coord); };
        }

    public:
        glm::mat3 m_transformMatrix = math::I3;
        glm::mat4 m_3dTransformMatrix = math::I4;
        glm::mat3 m_inverseTransformMatrix = math::I3;
    };
} // namespace RMC::utils