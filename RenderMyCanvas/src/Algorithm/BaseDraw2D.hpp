#pragma once
#include <functional>
#include <array>
#include <any>
#include <stdexcept>
#include <iostream>
#include <format>

#include "AlgoPredefines.hpp"

namespace RMC::algorithm {
    class BaseDraw2D
    {
    public:
        using uint64_t = unsigned long long;
        using Point = algorithm::Point<float, 2>;
        using ParamMap = std::unordered_map<std::string, std::any>;

    public:
        virtual ~BaseDraw2D() {}

        virtual void SetParams(const ParamMap& params) = 0;

        virtual void Draw() const = 0;
    };
}
