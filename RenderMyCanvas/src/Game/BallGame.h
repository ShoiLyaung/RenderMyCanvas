#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Material/Material.h"

#include "Game/Player.h"
#include "Game/Food.h"

#include "Network/Network.h"

namespace RMC {

    class BallGame {
    public:
        BallGame();
        void OnUpdate(float ts);
        void HandleInput();
        void UpdateOtherPlayer(std::string player_id, uint32_t x, uint32_t y, uint32_t z, uint32_t weight);
		void UpdateFood(std::string food_id, uint32_t x, uint32_t y, uint32_t z);
        std::vector<Player> Players;
        std::vector<Food> Foods;
        std::string m_playerID = "1001";
    private:
        glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };
        glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };
        glm::vec3 bigBallDirection; // 大球移动方向
        Network m_network;
        int frame_idx = 0;
        //void CheckCollision(Scene& scene);
    };

} // namespace RMC