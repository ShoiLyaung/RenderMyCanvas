#include "BallGame.h"
#include "Walnut/Input/Input.h"

namespace RMC {

	BallGame::BallGame() : bigBallDirection(0.0f)
	{
		// 初始化大球和小球
		Player player;
		player.MaterialIndex = 0;
		player.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		player.Radius = 1.0f;
		Players.push_back(player);


		// 生成一些小球
		for (int i = 0; i < 100; ++i) {
			Food smallBall;
			smallBall.MaterialIndex = 2;
			smallBall.Radius = 0.2f; // 小球半径
			smallBall.Position = glm::vec3(rand() % 100 - 50, 0.0f, rand() % 100 - 50); // 随机位置
			Foods.push_back(smallBall);
		}
	}

	void BallGame::OnUpdate(float ts) {
		HandleInput();
		//CheckCollision(scene);

		// 更新玩家位置
		for (auto& player : Players) {
			if (player.GetPlayerID() == m_playerID)
			{
				player.Position += bigBallDirection * player.GetSpeed() * ts;
				m_network->send_data(0, m_playerID, player.Position[0]*1000, player.Position[1] * 1000, player.Position[2] * 1000);
			}
		}
	}

	void BallGame::HandleInput() {
		bigBallDirection = glm::vec3(0.0f);

		if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left))
		{
			return;
		}

		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W)) bigBallDirection.z -= 1.0f;
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S)) bigBallDirection.z += 1.0f;
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A)) bigBallDirection.x -= 1.0f;
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D)) bigBallDirection.x += 1.0f;

		// 归一化方向
		if (glm::length(bigBallDirection) > 0.0f) {
			bigBallDirection = glm::normalize(bigBallDirection);
		}
	}

	void BallGame::UpdateOtherPlayer(std::string player_id, uint32_t x, uint32_t y, uint32_t z, uint32_t weight)
	{
		for (auto& player : Players)
		{
			if (player.playerID == player_id)
			{
				player.Position = glm::vec3(x / 1000.0, y / 1000.0, z / 1000.0);
				player.Radius = weight;
				return;
			}
		}
		Player new_player;
		new_player.playerID = player_id;
		new_player.Position = glm::vec3(x, y, z);
		Players.push_back(new_player);
	}

	//void BallGame::CheckCollision(Scene& scene) {
	//	// 检查大球和小球的碰撞
	//	for (size_t x = 0; x < scene.Spheres.size(); x++)
	//	{
	//		for (size_t y = 0; y < scene.Spheres.size(); y++)
	//		{
	//			float distance = glm::length(scene.Spheres[x].Position - scene.Spheres[y].Position);
	//			if (distance < (scene.Spheres[x].Radius + scene.Spheres[y].Radius) && x != y && scene.Spheres[x].Radius > scene.Spheres[y].Radius) {
	//				float r = scene.Spheres[y].Radius;
	//				scene.Spheres.erase(scene.Spheres.begin() + y);
	//				scene.Spheres[x].Radius += r;
	//			}
	//		}
	//	}
	//}

}