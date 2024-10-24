#include "CollisionSystem.h"
#include "Components/TransformComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/PlayerComponent.h"

void CollisionSystem::Update(entt::registry& registry)
{
    auto view = registry.view<TransformComponent, CollisionComponent>();

    for (auto entityA : view)
    {
        auto& transformA = view.get<TransformComponent>(entityA);
        auto& collisionA = view.get<CollisionComponent>(entityA);

        for (auto entityB : view)
        {
            if (entityA == entityB)
                continue;

            auto& transformB = view.get<TransformComponent>(entityB);
            auto& collisionB = view.get<CollisionComponent>(entityB);

            glm::vec3 delta = transformB.Position - transformA.Position;
            float distance = glm::length(delta);
            float collisionDistance = collisionA.Radius + collisionB.Radius;

            if (distance < collisionDistance)
            {
                // 处理碰撞逻辑
                bool entityAIsPlayer = registry.any_of<PlayerComponent>(entityA);
                bool entityBIsPlayer = registry.any_of<PlayerComponent>(entityB);

                if (entityAIsPlayer && !entityBIsPlayer)
                {
                    // 玩家吸收小球
                    collisionA.Radius += collisionB.Radius * 0.1f;
                    registry.destroy(entityB);
                }
                else if (!entityAIsPlayer && entityBIsPlayer)
                {
                    collisionB.Radius += collisionA.Radius * 0.1f;
                    registry.destroy(entityA);
                }
                else if (entityAIsPlayer && entityBIsPlayer)
                {
                    // 玩家之间的碰撞逻辑
                    if (collisionA.Radius > collisionB.Radius)
                    {
                        collisionA.Radius += collisionB.Radius * 0.1f;
                        registry.destroy(entityB);
                    }
                    else if (collisionA.Radius < collisionB.Radius)
                    {
                        collisionB.Radius += collisionA.Radius * 0.1f;
                        registry.destroy(entityA);
                    }
                }
            }
        }
    }
}