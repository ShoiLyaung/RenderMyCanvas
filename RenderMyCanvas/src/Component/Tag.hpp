#pragma once
#include "BaseComponent.hpp"

namespace RMC::component {
    class Tag : public BaseComponent
    {
    public:
        explicit Tag() = default;
        explicit Tag(const std::string& name) : m_name(name) {}
        virtual ~Tag() override {}

        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::Tag";
        }

        std::string toString() const { return m_name; }

    private:
        std::string m_name;
    };
} // namespace RMC::component