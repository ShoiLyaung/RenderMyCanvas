#pragma once

#include "Types.hpp"

// Do Not include "Utils.hpp"!

namespace RMC {
#if _HAS_CXX20 || __cplusplus >=202002L
#define RMC_REQUIRE(...) requires(__VA_ARGS__)

    template<typename T, typename = void>
    struct Decomposable : std::false_type {};

    template<typename T>
    struct Decomposable<T, std::void_t<decltype(std::tuple_size<T>::value)>> : std::true_type {};

    template<typename T>
    concept IsDecomposable = Decomposable<T>::value;

    template<typename T>
    concept IsPoint2D = requires(T point, decltype(point[0]) x, decltype(point[1]) y) {
        { decltype(point[0])(point[0]) } -> std::convertible_to<float>;
        { decltype(point[1])(point[1]) } -> std::convertible_to<float>;
        { T{ x, y } } -> std::same_as<T>;
    };

    template<typename T>
    concept IsPoint3D = requires(T point, decltype(point[0]) x, decltype(point[1]) y, decltype(point[2]) z) {
        { decltype(point[0])(point[0]) } -> std::convertible_to<float>;
        { decltype(point[1])(point[1]) } -> std::convertible_to<float>;
        { decltype(point[2])(point[2]) } -> std::convertible_to<float>;
        { T{ x, y, z } } -> std::same_as<T>;
    };

    template<typename T, typename E>
    concept IsRangeLvalue = IsDecomposable<T> && requires(T range) {
        { std::get<0>(range) }->std::same_as<E&>;
        { std::get<1>(range) }->std::same_as<E&>;
    };

    template<typename T, typename E>
    concept IsRangeRvalue = IsDecomposable<T> && requires(T && range) {
        { std::get<0>(std::move(range)) }->std::same_as<E>;
        { std::get<1>(std::move(range)) }->std::same_as<E>;
    };

    template<typename T, typename E>
    concept IsRangeConstLvalue = IsDecomposable<T> && requires(const T range) {
        { std::get<0>(range) }->std::same_as<const E&>;
        { std::get<1>(range) }->std::same_as<const E&>;
    };

    template<typename T, typename E>
    concept IsRange = IsRangeLvalue<T, E> || IsRangeRvalue<T, E> || IsRangeConstLvalue<T, E>;

#else
#define RMC_REQUIRE(...)

#endif
} // namespace RMC