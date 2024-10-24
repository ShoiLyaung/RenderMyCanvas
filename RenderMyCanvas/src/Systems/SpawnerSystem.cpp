#include "SpawnerSystem.h"
#include "Components/TransformComponent.h"
#include "Components/RenderComponent.h"
#include "Components/CollisionComponent.h"
#include <random>

float RandomFloat(float min, float max)
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(min, max); // rage 0 - 1
    return dis(e);
}

void SpawnerSystem::Update(entt::registry& registry, float deltaTime)
{
    static float timer = 0.0f;
    timer += deltaTime;

    if (timer >= 1.0f)
    {
        timer = 0.0f;

        // 生成新的小球
        auto entity = registry.create();

        TransformComponent transform;
        transform.Position = glm::vec3(
            RandomFloat(-100.0f, 100.0f),
            RandomFloat(-100.0f, 100.0f),
            RandomFloat(-100.0f, 100.0f)
        );

        CollisionComponent collision;
        collision.Radius = RandomFloat(0.5f, 2.0f);

        RenderComponent render;
        //render.Mesh = /* 您的小球网格 */;
        //render.Material = /* 您的小球材质 */;

        registry.emplace<TransformComponent>(entity, transform);
        registry.emplace<CollisionComponent>(entity, collision);
        registry.emplace<RenderComponent>(entity, render);
    }
}