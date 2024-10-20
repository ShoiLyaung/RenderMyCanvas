#pragma once
#include "Component/Material.hpp"
#include "Component/Transform.hpp"
#include "Component/VertexList.hpp"
#include "Primitive/BasePrimitive.hpp"
#include "Primitive/ControlPoint.hpp"
#include "Primitive/PrimitiveFactory.hpp"
#include "Utils/Types.hpp"

namespace RMC::scene
{
class Scene
{
    friend class SceneSerializer;

public:
    std::vector<std::shared_ptr<primitive::BasePrimitive>> entities{};
    std::vector<std::shared_ptr<primitive::BasePrimitive>> tempEntities{};
    std::vector<std::shared_ptr<primitive::BasePrimitive>> controlPoints{};
    std::vector<std::shared_ptr<primitive::BasePrimitive>> test_spheres{};
    std::vector<std::shared_ptr<primitive::BasePrimitive>> test_polygons{};
    std::vector<std::shared_ptr<component::Transform>> test_transforms{};
    std::vector<std::shared_ptr<component::Material>> test_materials{};
    std::vector<std::shared_ptr<component::VertexList>> test_vlists{};
    std::vector<std::shared_ptr<component::Transform>> test_transforms_p{};
    std::vector<std::shared_ptr<component::Material>> test_materials_p{};
    std::vector<std::shared_ptr<component::VertexList>> test_vlists_p{};
    primitive::PrimitiveFactory entityFactory{};

    bool isRayTracing = true;
};
}  // namespace RMC::scene