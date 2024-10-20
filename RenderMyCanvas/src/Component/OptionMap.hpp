#pragma once
#include "BaseComponent.hpp"

namespace RMC::component {
    class OptionMap : public BaseComponent
    {
        using MyBase = BaseComponent;
    public:
        explicit OptionMap() : optMap() {}
        explicit OptionMap(const std::unordered_map<std::string, std::string>& map) : optMap(map) {}

        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::OptionMap";
        }

        void set(const std::string& key, const std::string& value) { optMap[key] = value; }
        void set(const std::string& key, std::string&& value) { optMap[key] = std::move(value); }
        std::string& operator[](const std::string& key) { return optMap.at(key); }
        const std::string& operator[](const std::string& key) const { return optMap.at(key); }
        auto find(const std::string& key) { return optMap.find(key); }
        const auto find(const std::string& key) const { return optMap.find(key); }

        std::string& at(const std::string& key) { return optMap.at(key); }
        const std::string& at(const std::string& key) const { return optMap.at(key); }

        auto end() { return optMap.end(); }
        auto end() const { return optMap.cend(); }
        auto cend() const { return optMap.cend(); }

        auto begin() { return optMap.begin(); }
        auto begin() const { return optMap.cbegin(); }
        auto cbegin() const { return optMap.cbegin(); }

    public:
        std::unordered_map<std::string, std::string> optMap;
    };
}