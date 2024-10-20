#pragma once
#include "pch.hpp"
#include "Types.hpp"

namespace RMC::math {
    constexpr float PI = 3.1415927F;

    constexpr float Degree_to_Radian(float degree)
    {
        return degree * PI / 180.0F;
    }

    constexpr float Radian_to_Degree(float radian)
    {
        return radian * 180.0F / PI;
    }

    constexpr glm::mat3 I3 = {
        1.0F, 0.0F, 0.0F,
        0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 1.0F
    };

    constexpr glm::mat4 I4 = {
        1.0F, 0.0F, 0.0F, 0.0F,
        0.0F, 1.0F, 0.0F, 0.0F,
        0.0F, 0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    };

    constexpr glm::mat3 MatTranslate(const glm::vec2& offset)
    {
        return glm::mat3{
            1.0F, 0.0F, offset.x,
            0.0F, 1.0F, offset.y,
            0.0F, 0.0F,     1.0F
        };
    }

    constexpr glm::mat4 MatTranslate(const glm::vec3& offset)
    {
        return glm::mat4{
            1.0F, 0.0F, 0.0F, offset.x,
            0.0F, 1.0F, 0.0F, offset.y,
            0.0F, 0.0F, 1.0F, offset.z,
            0.0F, 0.0F, 0.0F,      1.0F
        };
    }

    constexpr glm::mat3 MatRotate(float degree)
    {
        float radian = Degree_to_Radian(degree);
        return glm::mat3{
            ::cos(radian), -::sin(radian), 0.0F,
            ::sin(radian),  ::cos(radian), 0.0F,
                     0.0F,           0.0F, 1.0F
        };
    }

    constexpr glm::mat4 MatRotate(glm::quat rotation)
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
        return result;
    }

    constexpr glm::mat3 MatFlip(int8_t dir)
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

    constexpr glm::mat3 MatScale(const glm::vec2& scale)
    {
        return glm::mat3{
            scale.x,    0.0F, 0.0F,
               0.0F, scale.y, 0.0F,
               0.0F,    0.0F, 1.0F
        };
    }

    constexpr glm::mat4 MatScale(const glm::vec3 & scale)
    {
        return glm::mat4{
            scale.x,    0.0F,    0.0F, 0.0F,
               0.0F, scale.y,    0.0F, 0.0F,
               0.0F,    0.0F, scale.z, 0.0F,
               0.0F,    0.0F,    0.0F, 1.0F
        };
    }

    constexpr uint32_t PCG_Hash(uint32_t x)
    {
        uint32_t state = x * 747796405u + 2891336453u;
        uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }

    constexpr static float Randomfloat(uint32_t& seed)
    {
        seed = PCG_Hash(seed);
        return float(seed) / float(std::numeric_limits<uint32_t>::max());
    }
}