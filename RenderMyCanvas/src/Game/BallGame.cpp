#include "BallGame.h"
#include "Walnut/Input/Input.h"

namespace RMC {

	BallGame::BallGame() : bigBallDirection(0.0f)
	{
		m_ForwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
		// 初始化大球和小球
		Player player;
		player.MaterialIndex = 0;
		player.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		player.Radius = 1.0f;
		Players.push_back(player);

		//// 生成一些小球
		//for (int i = 0; i < 100; ++i) {
		//	Food smallBall;
		//	smallBall.MaterialIndex = 2;
		//	smallBall.Radius = 0.2f; // 小球半径
		//	smallBall.Position = glm::vec3(rand() % 100 - 50, 0.0f, rand() % 100 - 50); // 随机位置
		//	Foods.push_back(smallBall);
		//}
	}

	void BallGame::OnUpdate(float ts) {
		frame_idx++;
		if (!m_network.game_started)
			return;
		m_playerID = m_network.m_playerID;
		Players[0].playerID = m_playerID;

		HandleInput();
		//CheckCollision(scene);

		// 更新玩家位置
		for (auto& player : Players) {
			if (player.GetPlayerID() == m_playerID)
			{
				player.Position += bigBallDirection * player.GetSpeed() * ts;
				m_network.send_data(frame_idx, m_playerID, player.Position[0]*1000, player.Position[1] * 1000, player.Position[2] * 1000);
			}
		}

		m_network.jsonObj_lock = true;
		// 访问并遍历 "players" 列表
		Player p = Players[0];
		Players.clear();
		Players.push_back(p);
		for (const auto& player : m_network.players)
		{
			if (player.id == m_playerID)
			{
				Players[0].Radius = player.weight;
				continue;
			}
			UpdateOtherPlayer(player.id, player.pos, player.weight);
		}

		Foods.clear();
		for (const auto& food : m_network.foods)
		{
			UpdateFood(food.id, food.pos);
		}
		m_network.jsonObj_lock = false;
	}

	void BallGame::HandleInput() {
		bigBallDirection = glm::vec3(0.0f);
		glm::vec2 mousePos = Walnut::Input::GetMousePosition();
		glm::vec2 delta = (mousePos - m_LastMousePosition) * 0.002f;
		m_LastMousePosition = mousePos;

		if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left))
		{
			return;
		}
		constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
		glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

		// Movement
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W))
		{
			bigBallDirection += m_ForwardDirection;
		}
		else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S))
		{
			bigBallDirection -= m_ForwardDirection;
		}
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A))
		{
			bigBallDirection -= rightDirection;
		}
		else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D))
		{
			bigBallDirection += rightDirection;
		}
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Q))
		{
			bigBallDirection -= upDirection;
		}
		else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::E))
		{
			bigBallDirection += upDirection;
		}

		// Rotation
		if (delta.x != 0.0f || delta.y != 0.0f)
		{
			float pitchDelta = delta.y * 0.3;
			float yawDelta = delta.x * 0.3;

			glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
				glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
			m_ForwardDirection = glm::rotate(q, m_ForwardDirection);
		}

		// 归一化方向
		if (glm::length(bigBallDirection) > 0.0f) {
			bigBallDirection = glm::normalize(bigBallDirection);
		}
	}

	void BallGame::UpdateOtherPlayer(std::string player_id, glm::vec3 position, float weight)
	{
		Player new_player;
		new_player.playerID = player_id;
		new_player.Position = position;
		new_player.Radius = weight;
		new_player.MaterialIndex = 5;
		Players.push_back(new_player);
	}

	void BallGame::UpdateFood(std::string food_id, glm::vec3 position)
	{
		Food new_food;
		new_food.foodID = food_id;
		new_food.Position = position;
		new_food.Radius = 0.2f;
		new_food.MaterialIndex = std::stoi(food_id) % 10 + 1;
		Foods.push_back(new_food);
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