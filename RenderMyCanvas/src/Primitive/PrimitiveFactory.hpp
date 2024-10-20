#pragma once
#include "BasePrimitive.hpp"
#include "Utils/Utils.hpp"

namespace RMC::primitive {
    class PrimitiveFactory
    {
    public:
        using CreateFunc = std::function<std::shared_ptr<BasePrimitive>()>;

        void Register(const std::string& name, CreateFunc func)
        {
            RMC_INFO("Register Primitive: {}", name);
            m_factoryMap[name] = func;
        }

        std::shared_ptr<BasePrimitive> Create(const std::string& name)
        {
            auto it = m_factoryMap.find(name);
            if (it != m_factoryMap.end()) {
                return it->second();
            } else {
                throw std::runtime_error("Primitive Name Not Found");
            }
            return nullptr;
        }

    private:
        HashMap<std::string, CreateFunc> m_factoryMap;
    };
} // namespace RMC::primitive