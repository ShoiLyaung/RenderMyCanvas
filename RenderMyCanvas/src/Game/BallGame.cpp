#include "BallGame.h"
#include "Walnut/Input/Input.h"

namespace RMC {

	BallGame::BallGame() : bigBallDirection(0.0f)
	{
		m_ForwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
		// 初始化大球和小球
		Player player;
		player.MaterialIndex = 7;
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

		// 访问并遍历 "players" 列表
		if (m_network.m_jsonObj.contains("data") && m_network.m_jsonObj["data"].contains("players") && m_network.m_jsonObj["data"]["players"].is_array()) {
			std::cout << "Players:" << std::endl;
			for (const auto& player : m_network.m_jsonObj["data"]["players"]) {
				std::string id = player["id"];
				int weight = player["weight"];
				bool alive = player["alive"];
				auto pos = player["pos"];

				if (id == m_playerID)
					continue;
				// 输出玩家信息
				UpdateOtherPlayer(id, pos[0], pos[1], pos[2],weight);
			}
		}
		else {
			//std::cout << "No players found in JSON data." << std::endl;
		}
		std::cout << "Food LEN:" << Foods.size() << std::endl;

		// 访问并遍历 "foods" 列表
		if (m_network.m_jsonObj.contains("data") && m_network.m_jsonObj["data"].contains("foods") && m_network.m_jsonObj["data"]["foods"].is_array()) {
			std::cout << "Foods:" << std::endl;
			for (const auto& food : m_network.m_jsonObj["data"]["foods"]) {
				std::string id = food["id"];
				auto pos = food["pos"];

				// 输出食物信息
				UpdateFood(id, pos[0], pos[1], pos[2]);	
			}
		}
		else {
			//std::cout << "No foods found in JSON data." << std::endl;
		}
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

	void BallGame::UpdateOtherPlayer(std::string player_id, uint32_t x, uint32_t y, uint32_t z, uint32_t weight)
	{
		for (auto& player : Players)
		{
			if (player.playerID == player_id)
			{
				player.Position = glm::vec3(x / 1000.0, y / 1000.0, z / 1000.0);
				player.Radius = weight/1000.0;
				return;
			}
		}
		Player new_player;
		new_player.playerID = player_id;
		new_player.Position = glm::vec3(x / 1000.0, y / 1000.0, z / 1000.0);
		new_player.Radius = weight;
		new_player.MaterialIndex = 7;
		Players.push_back(new_player);
	}

	void BallGame::UpdateFood(std::string food_id, uint32_t x, uint32_t y, uint32_t z)
	{
		for (auto& food : Foods)
		{
			if (food.foodID == food_id)
			{
				food.Position = glm::vec3(x / 1000.0, y / 1000.0, z / 1000.0);
				return;
			}
		}
		Food new_food;
		new_food.foodID = food_id;
		new_food.Position = glm::vec3(x / 1000.0, y / 1000.0, z / 1000.0);
		new_food.Radius = 0.2f;
		new_food.MaterialIndex = 2;
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