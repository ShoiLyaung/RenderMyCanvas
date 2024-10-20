#pragma once
#include "BaseComponent.hpp"
#include "Algorithm/BaseDraw2D.hpp"

namespace RMC::component {
    class DrawAlgo2D : public BaseComponent
    {
    public:
        using Point = algorithm::BaseDraw2D::Point;
        using ParamMap = algorithm::BaseDraw2D::ParamMap;

    public:
        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::DrawAlgo2D";
        }

        void SetParams(const ParamMap& params)
        {
            algo->SetParams(params);
        }

        void Draw() const
        {
            algo->Draw();
        }

        template<typename AlgoT>
            requires std::is_base_of_v<algorithm::BaseDraw2D, AlgoT>
        void SetAlgorithm()
        {
            algo = std::make_shared<AlgoT>();
        }

        void RemoveAlgorithm()
        {
            algo = nullptr;
        }

    public:
        std::shared_ptr<algorithm::BaseDraw2D> algo = nullptr;
    };
}