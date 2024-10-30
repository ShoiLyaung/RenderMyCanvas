#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Scene/Scene.h"
#include "Primitives/Sphere.h"
#include "Material/Material.h"

namespace RMC {

    class BallGame {
    public:
        BallGame(Scene& scene);
        void OnUpdate(float ts, Scene& scene);
        void HandleInput();

    private:
        glm::vec3 bigBallDirection; // 大球移动方向
        float bigBallSpeed; // 大球速度

        void CheckCollision(Scene& scene);
    };

} // namespace RMC