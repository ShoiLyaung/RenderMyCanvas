#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Material/Material.h"

#include "Game/Player.h"
#include "Game/Food.h"

namespace RMC {

    class BallGame {
    public:
        BallGame();
        void OnUpdate(float ts);
        void HandleInput();
        std::vector<Player> Players;
        std::vector<Food> Foods;
    private:
        std::string m_playerID = "1001";
        glm::vec3 bigBallDirection; // 大球移动方向

        //void CheckCollision(Scene& scene);
    };

} // namespace RMC