#pragma once
#include "pch.hpp"

#include "Concepts.hpp"
#include "Log.hpp"
#include "Types.hpp"

#include "Math.hpp"

namespace RMC::utils
{
constexpr uint32_t RGBA_to_uint32_t(const glm::vec4& rgbaColor)
{
    return (static_cast<uint32_t>(rgbaColor.r * 255)) |
           (static_cast<uint32_t>(rgbaColor.g * 255) << 8) |
           (static_cast<uint32_t>(rgbaColor.b * 255) << 16) |
           (static_cast<uint32_t>(1 * 255) << 24);
}

constexpr glm::vec4 uint32_t_to_RGBA(uint32_t rgbaColor)
{
    return {static_cast<float>(rgbaColor & 0xFF) / 255.0F,
            static_cast<float>((rgbaColor >> 8) & 0xFF) / 255.0F,
            static_cast<float>((rgbaColor >> 16) & 0xFF) / 255.0F,
            static_cast<float>((rgbaColor >> 24) & 0xFF) / 255.0F};
}

template <typename T>
    requires std::is_arithmetic_v<T>
constexpr int64_t Round(T x)
{
    return int64_t(x + 0.5f);
}

template <typename... Ts>
constexpr auto Round(Ts... xs)
{
    return std::make_tuple(Round(xs)...);
}

/**
 * @brief Get quaternion of rotation from {orig} to {dest}.
 *
 * @param dest - The destination direction, does not need to be normalized.
 * @param orig - The original direction, does not need to be normalized.
 */
inline glm::quat GetRotationQuat(const glm::vec3& dest,
                               const glm::vec3& orig = {1.0f, 0.0f, 0.0f})
{
    if (dest.x == 0.0F && dest.y == 0.0F) {
        return glm::quat{1.0F, 0.0F, 0.0F, 0.0F};
    }
    return glm::rotation(glm::normalize(orig), glm::normalize(dest));
}

/**
 * @brief Check if {x} is in range of [left, right)
 */
template <typename T1, typename T2, typename T3,
          typename Comp = std::less<void>>
constexpr bool IsInRange(T1 x, T2 left, T3 right, Comp comp = {})
{
    return (!comp(x, left) && comp(x, right));
}

template <typename T1, typename T2, typename T3>
    requires std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> &&
             std::is_arithmetic_v<T3>
constexpr uint64_t To1DIndex(T1 x, T2 y, T3 width)
{
    return utils::Round(x) + uint64_t(width) * utils::Round(y);
}

template <typename Point, typename T>
    requires IsPoint2D<Point> && std::is_arithmetic_v<T>
constexpr uint64_t To1DIndex(const Point& point, T width)
{
    return To1DIndex(point[0], point[1], width);
}
}  // namespace RMC::utils