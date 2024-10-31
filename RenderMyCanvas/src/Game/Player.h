#pragma once
#include <string>
#include "Primitives/Sphere.h"

namespace RMC {
    class Player : public Sphere {
    private:
    public:
		Player() = default;
        Player(float s) : speed(s) {}
        ~Player() = default;

        float speed = 5.0f;     // 速度
        std::string playerID = "1001"; // 玩家ID

        // 访问者函数
        float GetSpeed() const { return speed; }
		std::string GetPlayerID() const { return playerID; }
    };
}