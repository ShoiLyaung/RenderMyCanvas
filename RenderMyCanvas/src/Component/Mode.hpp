#pragma once
#include "BaseComponent.hpp"

namespace RMC::component {
    class Mode : public BaseComponent
    {
    public:
        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::Mode";
        }

        explicit Mode(uint8_t m = 0) : mode(m) {}
    public:
        uint8_t mode;
    };
}