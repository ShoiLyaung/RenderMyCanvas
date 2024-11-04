#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

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
        std::vector<Player> Players;
        std::vector<Food> Foods;
        std::string m_playerID = "1001";
    private:
        glm::vec3 bigBallDirection; // 大球移动方向
        Network *m_network;
        //void CheckCollision(Scene& scene);
    };

} // namespace RMC