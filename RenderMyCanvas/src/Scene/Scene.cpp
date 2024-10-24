#include "Scene.h"
#include "Components/TransformComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/PlayerComponent.h"
#include "Systems/MovementSystem.h"
#include "Systems/CollisionSystem.h"

namespace RMC {
    Scene::Scene()
    {
        // �������ʵ��
        entt::entity player = m_Registry.create();
        TransformComponent playerTransform;
        playerTransform.Position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Registry.emplace<TransformComponent>(player, playerTransform);

        SphereComponent playerSphere;
        playerSphere.Radius = 1.0f;
        m_Registry.emplace<SphereComponent>(player, playerSphere);

        MaterialComponent playerMaterial;
        playerMaterial.Albedo = glm::vec3(0.8f, 0.1f, 0.1f); // ��ɫ
        m_Registry.emplace<MaterialComponent>(player, playerMaterial);

        m_Registry.emplace<PlayerComponent>(player);

        // ����һЩС��ʵ��
        for (int i = 0; i < 10; ++i)
        {
            entt::entity ball = m_Registry.create();

            TransformComponent ballTransform;
            ballTransform.Position = glm::vec3(
                RandomFloat(-15.0f, 15.0f),
                RandomFloat(-15.0f, 15.0f),
                RandomFloat(-15.0f, 15.0f)
            );
            m_Registry.emplace<TransformComponent>(ball, ballTransform);

            SphereComponent ballSphere;
            ballSphere.Radius = RandomFloat(0.5f, 1.0f);
            m_Registry.emplace<SphereComponent>(ball, ballSphere);

            MaterialComponent ballMaterial;
            ballMaterial.Albedo = glm::vec3(
                RandomFloat(0.0f, 1.0f),
                RandomFloat(0.0f, 1.0f),
                RandomFloat(0.0f, 1.0f)
            );
            m_Registry.emplace<MaterialComponent>(ball, ballMaterial);
        }

        // �����Ҫ�����Դ����ϰ���ʵ��
    }

	Scene::~Scene()
	{
		// ����ϵͳ
		// MovementSystem::Update(m_Registry, deltaTime);
		// CollisionSystem::Update(m_Registry);
		// SpawnerSystem::Update(m_Registry, deltaTime);
	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	bool Scene::OnUpdate(float ts)
	{
        MovementSystem::Update(m_Registry, ts);
        CollisionSystem::Update(m_Registry);
		return 0;
	}

    // ��������ɺ���
    float Scene::RandomFloat(float min, float max)
    {
        static std::default_random_engine e;
        static std::uniform_real_distribution<float> dis(min, max);
        return dis(e);
    }
} // namespace RMC