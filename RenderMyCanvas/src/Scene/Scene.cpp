#include "Scene.h"

namespace RMC {
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(float ts)
	{
	}
} // namespace RMC