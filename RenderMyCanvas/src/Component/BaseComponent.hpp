#pragma once
#include "../Utils/Utils.hpp"

namespace RMC::component {
    class BaseComponent
    {
    public:
        explicit BaseComponent() = default;
        virtual ~BaseComponent() {}
        virtual std::string WhatAmI() const = 0;
    };
} // namespace RMC::component