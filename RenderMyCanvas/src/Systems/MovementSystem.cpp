#include "MovementSystem.h"
#include "Components/TransformComponent.h"
#include "Components/MovementComponent.h"

void MovementSystem::Update(entt::registry& registry, float deltaTime)
{
    auto view = registry.view<TransformComponent, MovementComponent>();

    for (auto entity : view)
    {
        auto& transform = view.get<TransformComponent>(entity);
        auto& movement = view.get<MovementComponent>(entity);

        movement.Velocity += movement.Acceleration * deltaTime;
        transform.Position += movement.Velocity * deltaTime;

        // ���ü��ٶ�
        movement.Acceleration = glm::vec3(0.0f);
    }
}