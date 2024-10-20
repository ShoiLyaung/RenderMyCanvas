#include "pch.hpp"
#include "CoordTransformer.hpp"

namespace RMC::utils {
    glm::mat3 CoordTransformer::MatTranslate(const glm::vec2& offset) const
    {
        return glm::mat3{
            1.0F, 0.0F, offset.x,
            0.0F, 1.0F, offset.y,
            0.0F, 0.0F,     1.0F
        };
    }

    glm::mat4 CoordTransformer::MatTranslate(const glm::vec3& offset) const
    {
        return glm::mat4{
            1.0F, 0.0F, 0.0F, offset.x,
            0.0F, 1.0F, 0.0F, offset.y,
            0.0F, 0.0F, 1.0F, offset.z,
            0.0F, 0.0F, 0.0F,      1.0F
        };
    }

    glm::mat3 CoordTransformer::MatRotate(float degree) const
    {
        float radian = math::Degree_to_Radian(degree);
        return glm::mat3{
            ::cos(radian), -::sin(radian), 0.0F,
            ::sin(radian),  ::cos(radian), 0.0F,
                     0.0F,           0.0F, 1.0F
        };
    }

    glm::mat4 CoordTransformer::MatRotate(glm::quat rotation) const
    {
        glm::mat4 result{};
        result[0][0] = 1.0F - 2.0F * rotation.y * rotation.y - 2.0F * rotation.z * rotation.z;
        result[0][1] = 2.0F * rotation.x * rotation.y - 2.0F * rotation.z * rotation.w;
        result[0][2] = 2.0F * rotation.x * rotation.z + 2.0F * rotation.y * rotation.w;
        result[0][3] = 0.0F;
        result[1][0] = 2.0F * rotation.x * rotation.y + 2.0F * rotation.z * rotation.w;
        result[1][1] = 1.0F - 2.0F * rotation.x * rotation.x - 2.0F * rotation.z * rotation.z;
        result[1][2] = 2.0F * rotation.y * rotation.z - 2.0F * rotation.x * rotation.w;
        result[1][3] = 0.0F;
        result[2][0] = 2.0F * rotation.x * rotation.z - 2.0F * rotation.y * rotation.w;
        result[2][1] = 2.0F * rotation.y * rotation.z + 2.0F * rotation.x * rotation.w;
        result[2][2] = 1.0F - 2.0F * rotation.x * rotation.x - 2.0F * rotation.y * rotation.y;
        result[2][3] = 0.0F;
        result[3][0] = 0.0F;
        result[3][1] = 0.0F;
        result[3][2] = 0.0F;
        result[3][3] = 1.0F;
        if (result != glm::toMat4(rotation)) {
            throw std::runtime_error("Rotation matrix not equal!");
        }
        return result;
    }

    glm::mat3 CoordTransformer::MatFlip(int8_t dir) const
    {
        if (dir == 0) {
            return glm::mat3{
                -1.0F,  0.0F, 0.0F,
                 0.0F,  1.0F, 0.0F,
                 0.0F,  0.0F, 1.0F
            };
        } else if (dir == 1) {
            return glm::mat3{
                 1.0F,  0.0F, 0.0F,
                 0.0F, -1.0F, 0.0F,
                 0.0F,  0.0F, 1.0F
            };
        } else if (dir == 2) { // Exchange x and y
            return glm::mat3{
                 0.0F,  1.0F, 0.0F,
                 1.0F,  0.0F, 0.0F,
                 0.0F,  0.0F, 1.0F
            };
        }
    }

    glm::mat3 CoordTransformer::MatScale(const glm::vec2& scale) const
    {
        return glm::mat3{
            scale.x,    0.0F, 0.0F,
               0.0F, scale.y, 0.0F,
               0.0F,    0.0F, 1.0F
        };
    }

    glm::mat4 CoordTransformer::MatScale(const glm::vec3 & scale) const
    {
        return glm::mat4{
            scale.x,    0.0F,    0.0F, 0.0F,
               0.0F, scale.y,    0.0F, 0.0F,
               0.0F,    0.0F, scale.z, 0.0F,
               0.0F,    0.0F,    0.0F, 1.0F
        };
    }
}