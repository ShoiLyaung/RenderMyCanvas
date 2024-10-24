#include "InputSystem.h"
#include "Components/MovementComponent.h"
#include "Components/PlayerComponent.h"
#include "Walnut/Input/Input.h" // 您需要实现Input类来处理输入

void InputSystem::Update(entt::registry& registry, float deltaTime)
{
    auto view = registry.view<MovementComponent, PlayerComponent>();

    for (auto entity : view)
    {
        auto& movement = view.get<MovementComponent>(entity);

        glm::vec3 direction(0.0f);

        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W))
            direction.z -= 1.0f;
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S))
            direction.z += 1.0f;
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A))
            direction.x -= 1.0f;
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D))
            direction.x += 1.0f;
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Space))
            direction.y += 1.0f;
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::LeftShift))
            direction.y -= 1.0f;

        if (glm::length(direction) > 0.0f)
            direction = glm::normalize(direction);

        movement.Acceleration += direction * movement.Speed;
    }
}