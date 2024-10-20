#pragma once

#include "BaseComponent.hpp"
#include "../Utils/Types.hpp"

namespace RMC::component {
    class Material : public BaseComponent
    {
    public:
        explicit Material(
            const glm::vec4& albedo = glm::vec4{ 1.0f },
            float roughness = 1.0f,
            float metallic = 0.0f,
            const glm::vec4& emissionColor = glm::vec4{ 0.0f },
            float emissionPower = 0.0f
        ) : Albedo(albedo),
            Roughness(roughness),
            Metallic(metallic),
            EmissionColor(emissionColor),
            EmissionPower(emissionPower) {}

        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::Material";
        }

        virtual ~Material() {}

        glm::vec3 GetEmission() const
        {
            return EmissionColor * EmissionPower;
        }

        glm::vec4 Albedo;
        float Roughness;
        float Metallic;
        glm::vec3 EmissionColor;
        float EmissionPower;
    };
} // namespace RMC::component