#pragma once
#include "pch.hpp"

// Do Not include "Utils.hpp"!

namespace RMC
{
template <typename T>
using Ptr = T*;

template <typename T>
using ConstPtr = const T*;

template <typename T>
using Ref = T&;

template <typename T>
using Coord2d = glm::vec<2, T>;

template <typename T>
using Coord3d = glm::vec<3, T>;

template <typename... Ts>
using Tuple = std::tuple<Ts...>;

using Arr2F32 = std::array<float, 2>;
using Arr3F32 = std::array<float, 3>;

template <typename T, uint64_t size>
using Range = std::array<T, size>;

template <uint64_t size>
using RangeF32 = Range<float, size>;

using Range2F32 = Range<float, 2>;

using RangeU32 = Range<std::uint32_t, 2>;

using RangeU64 = Range<std::uint64_t, 2>;

template <typename T>
using StdVec = std::vector<T>;

using String = std::string;

template <typename K, typename E>
using Map = std::map<K, E>;

template <typename K, typename E>
using HashMap = std::unordered_map<K, E>;

template <typename T>
using List = std::list<T>;

template <size_t Dim, typename T>
using Vec = glm::vec<Dim, T>;
template <typename T>
using Vec2 = glm::tvec2<T>;

using PointType = std::variant<glm::vec2, glm::vec3>;
}  // namespace RMC