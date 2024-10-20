#pragma once
#include "Primitive/BasePrimitive.hpp"
#include "BaseComponent.hpp"
#pragma warning(disable : 4018)

namespace RMC::component {
    class Artist : public BaseComponent
    {
    public:
        virtual std::string WhatAmI() const override final
        {
            return "RMC::component::Artist";
        }

        template<typename T>
        void PutPoint(const T& point) const
        {
            float x = point[0];
            float y = point[1];
            auto& [width, height] = imageRange;

            if (buffImage == nullptr) { return; }

            // ||ToDo|| [23/Oct/19]
            //    Here actually draws a square, which is slow!
            for (uint64_t i = utils::Round(x - thickness / 2.0f); i < utils::Round(x + thickness / 2.0f); ++i) {
                for (uint64_t j = utils::Round(y - thickness / 2.0f); j < utils::Round(y + thickness / 2.0f); ++j) { // Retrun if point out of bound
                    if (!(utils::IsInRange(i, 0, width) && utils::IsInRange(j, 0, height))) { return; }
                    buffImage->at(utils::To1DIndex(i, j, width)) = color;
                    buffPrimitivePerPixel->at(utils::To1DIndex(i, j, width)) = whichPrimitive;
                }
            }
        }

        /**
         * @brief A simple encapsulation of `this->PutPoint<T>`.
         *        You should create your own encapsulation if you want color-changing or other effects.
         * @tparam T - Point type which should have operator[] at least.
         * @return - A std::function object.
        */
        template<typename T>
        auto GetPutPointFunc() -> std::function<void(const T&)> const
        {
            return [this](const T& point) { PutPoint(point); };
        }

    public:
        Ptr<std::vector<uint32_t>>                  buffImage = nullptr;
        Ptr<std::vector<std::shared_ptr<primitive::BasePrimitive>>> buffPrimitivePerPixel = nullptr;
        Range2F32                                 imageRange{};  // (width, height)
        uint32_t                                    color = 0xFFFFFFFF;
        uint32_t                                    thickness = 1;
        std::shared_ptr<primitive::BasePrimitive>       whichPrimitive = nullptr;
    };
}