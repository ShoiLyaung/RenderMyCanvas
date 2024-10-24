#include "RenderingSystem.h"
#include "Components/TransformComponent.h"
#include "Components/RenderComponent.h"

void RenderingSystem::Render(entt::registry& registry, RMC::Renderer& renderer, const RMC::Camera& camera)
{
    auto view = registry.view<TransformComponent, RenderComponent>();

    for (auto entity : view)
    {
        auto& transform = view.get<TransformComponent>(entity);
        //auto& renderComponent = view.get<RenderComponent>(entity);

        // 使用renderer来渲染实体
        //renderer.DrawMesh(renderComponent.Mesh, renderComponent.Material, transform);
    }
}