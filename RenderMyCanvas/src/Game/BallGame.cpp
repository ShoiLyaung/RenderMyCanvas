#include "BallGame.h"
#include "Walnut/Input/Input.h"

namespace RMC {

	BallGame::BallGame(Scene& scene) : bigBallDirection(0.0f), bigBallSpeed(0.0f)
	{
		std::vector<glm::vec3> predefinedColors = {
	{0.9f, 0.6f, 0.7f},
	{0.4f, 0.6f, 0.9f},
	{0.6f, 0.9f, 0.7f},
	{0.9f, 0.9f, 0.5f},
	{0.8f, 0.5f, 0.4f},
	{0.5f, 0.8f, 0.9f},
	{0.7f, 0.8f, 0.4f},
	{0.9f, 0.7f, 0.4f},
	{0.9f, 0.5f, 0.8f},
	{0.6f, 0.4f, 0.9f},
	{0.2f, 0.8f, 0.6f},
	{0.4f, 0.7f, 0.9f},
	{0.8f, 0.7f, 0.3f},
	{0.6f, 0.3f, 0.4f},
	{0.7f, 0.3f, 0.6f},
	{0.3f, 0.6f, 0.4f},
	{0.7f, 0.9f, 0.6f},
	{0.9f, 0.4f, 0.6f},
	{0.5f, 0.6f, 0.9f},
	{0.6f, 0.9f, 0.4f},
	{0.3f, 0.4f, 0.7f}
		};

		for (int i = 0; i < 3; i++) {
			Material& randomMaterial = scene.Materials.emplace_back();
			randomMaterial.Albedo = predefinedColors[rand() % predefinedColors.size()];
			randomMaterial.Roughness = static_cast<float>(rand()) / RAND_MAX * 1.0f;
			randomMaterial.Metallic = 0.0f;
			if (rand() % 2 == 0) {
				randomMaterial.EmissionColor = randomMaterial.Albedo * 0.5f;
				randomMaterial.EmissionPower = 1.5f;
			}
		}

		for (int i = 0; i < 3; i++) {
			Material& randomMaterial = scene.Materials.emplace_back();
			randomMaterial.Albedo = predefinedColors[rand() % predefinedColors.size()];
			randomMaterial.Roughness = static_cast<float>(rand()) / RAND_MAX * 0.2f;
			randomMaterial.Metallic = static_cast<float>(rand()) / RAND_MAX * 0.2f + 0.8f;
		}

		for (int i = 0; i < 4; i++) {
			Material& randomMaterial = scene.Materials.emplace_back();
			randomMaterial.Albedo = predefinedColors[rand() % predefinedColors.size()];
			randomMaterial.Roughness = static_cast<float>(rand()) / RAND_MAX * 1.0f;
			randomMaterial.Metallic = 0.0f;
		}

		// 初始化大球和小球
		Sphere Player;
		Player.MaterialIndex = 5;
		Player.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		Player.Radius = 0.5f;
		scene.Spheres.insert(scene.Spheres.begin()+ scene.playerID, Player);

		bigBallSpeed = 5.0f;

		// 生成一些小球
		for (int i = 0; i < 100; ++i) {
			Sphere smallBall;
			smallBall.Radius = 0.2f; // 小球半径
			smallBall.Position = glm::vec3(rand() % 100 - 50, 0.0f, rand() % 100 - 50); // 随机位置
			scene.Spheres.push_back(smallBall);
		}
	}

	void BallGame::OnUpdate(float ts, Scene& scene) {
		HandleInput();
		CheckCollision(scene);

		// 更新大球位置
		Sphere& bigBall = scene.Spheres[scene.playerID];
		bigBall.Position += bigBallDirection * bigBallSpeed * ts;
	}

	void BallGame::HandleInput() {
		bigBallDirection = glm::vec3(0.0f);

		if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Right))
		{
			Walnut::Input::SetCursorMode(Walnut::CursorMode::Normal);
			return;
		}

		Walnut::Input::SetCursorMode(Walnut::CursorMode::Locked);

		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W)) bigBallDirection.z -= 1.0f;
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S)) bigBallDirection.z += 1.0f;
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A)) bigBallDirection.x -= 1.0f;
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D)) bigBallDirection.x += 1.0f;

		// 归一化方向
		if (glm::length(bigBallDirection) > 0.0f) {
			bigBallDirection = glm::normalize(bigBallDirection);
		}
	}

	void BallGame::CheckCollision(Scene& scene) {
		// 检查大球和小球的碰撞
		for (size_t x = 0; x < scene.Spheres.size(); x++)
		{
			for (size_t y = 0; y < scene.Spheres.size(); y++)
			{
				float distance = glm::length(scene.Spheres[x].Position - scene.Spheres[y].Position);
				if (distance < (scene.Spheres[x].Radius + scene.Spheres[y].Radius) && x != y && scene.Spheres[x].Radius > scene.Spheres[y].Radius) {
					float r = scene.Spheres[y].Radius;
					scene.Spheres.erase(scene.Spheres.begin() + y);
					scene.Spheres[x].Radius += r;
				}
			}
		}
	}

}