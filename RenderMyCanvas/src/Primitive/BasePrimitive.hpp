#pragma once
#include "Utils/Utils.hpp"
#include "Renderer/Ray.hpp"
#include "Component/Transform.hpp"
#include "Component/VertexList.hpp"

#include "Component/Tag.hpp"
#include "Camera/Camera.hpp"

namespace RMC::primitive {
    class BasePrimitive
    {
    public:
        explicit BasePrimitive() : m_id{ 0 }, m_componentMap{} {}
        virtual ~BasePrimitive() = default;

        virtual void Draw(Ptr<const camera::Camera> camera) {};

        // @brief Get distance from ray origin to the primitive; If not hit, return -1.
        //
        // @note I don't want each primitive get their components with dynamic_cast (which slows down the program),
        //       so I pass necessary params to function `Hit` directly.
        virtual float Hit(const renderer::Ray& ray, const component::Transform& trsf, const component::VertexList& vlist) const = 0;

        ////virtual renderer::HitPayload Hit(const renderer::Ray& ray) { return renderer::MISS_PAYLOAD; }

        template<class T>
            requires std::is_base_of_v<component::BaseComponent, T>
        std::shared_ptr<T> GetComponent() const;

        template<typename T, typename... Args>
            requires std::is_base_of_v<component::BaseComponent, T>
        std::shared_ptr<T> AddComponent(Args&&... args);

        template <typename... Ts>
        void AddComponents() { (AddComponent<Ts>(), ...); }

        template<class T>
            requires std::is_base_of_v<component::BaseComponent, T>
        void RemoveComponent();

        const auto& GetComponentMap() const { return m_componentMap; }

        auto& GetComponentMap() { return m_componentMap; }


    protected:
        uint64_t   m_id; // Unique ID of the primitive
        std::unordered_map<
            std::string,
            std::shared_ptr<component::BaseComponent>
        > m_componentMap; // A vector of shared_ptr to components
    };

    template<class T>
        requires std::is_base_of_v<component::BaseComponent, T>
    inline std::shared_ptr<T> BasePrimitive::GetComponent() const
    {
        auto it = m_componentMap.find(T().WhatAmI());
        if (it == m_componentMap.end()) {
            return nullptr;
        } else {
            return std::dynamic_pointer_cast<T>(it->second);
        }
    }

    template<typename T, typename... Args>
        requires std::is_base_of_v<component::BaseComponent, T>
    inline std::shared_ptr<T> BasePrimitive::AddComponent(Args&&... args)
    {
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        m_componentMap[component->WhatAmI()] = component;
        return component;
    }

    template<class T>
        requires std::is_base_of_v<component::BaseComponent, T>
    inline void BasePrimitive::RemoveComponent()
    {
        auto it = m_componentMap.find(T().WhatAmI());
        if (it != m_componentMap.end()) {
            m_componentMap.erase(it);
        }
    }
} // namespace RMC::primitive