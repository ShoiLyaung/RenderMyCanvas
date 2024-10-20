#pragma once
#include "Utils/Utils.hpp"

namespace RMC::camera {
    class Camera
    {
    public:
        Camera(float verticalFOV, float nearClip, float farClip);

        bool OnUpdate(float ts);
        void OnResize(uint32_t width, uint32_t height);
        Range2F32 GetViewportSize() const { return { float(m_ViewportWidth), float(m_ViewportHeight) }; }

        const glm::mat4& GetProjection() const { return m_Projection; }
        const glm::mat4& GetInverseProjection() const { return m_InverseProjection; }
        const glm::mat4& GetView() const { return m_View; }
        const glm::mat4& GetInverseView() const { return m_InverseView; }
        const glm::vec3& GetPosition() const { return m_position; }
        const glm::vec3& GetForwardDirection() const { return m_forwardDirection; }
        const glm::quat& GetRotationQuat() const { return m_rotation; }
        const float  GetSpeed() const{ return m_speed; }
        const std::vector<glm::vec3>& GetRayDirections() const { return m_rayDirections; }

        void SetPosition(glm::vec3 position) { m_position = position; }

        float GetRotationSpeed();

        /**
         * @brief Project a point from screen space to world space.
         *
         * @param screenPoint - The point in screen space to project.
         * @param forwardDistance - The forward distance from the camera to project the point.
         * @return The point in world space.
        */
        glm::vec3 ScreenToWorld(Ref<const glm::vec2> screenPoint, float forwardDistance = 1) const;

        /**
         * @brief Project a point from world space to screen space.
         *
         * @param point - The point in world space to project.
         * @return The point in screen space.
        */
        glm::vec2 WorldToScreen(Ref<const glm::vec3> point) const;

    private:
        void RecalculateProjection();
        void RecalculateView();
        void RecalculateRayDirections();

    private:
        glm::mat4 m_Projection{ 1.0f };
        glm::mat4 m_View{ 1.0f };
        glm::mat4 m_InverseProjection{ 1.0f };
        glm::mat4 m_InverseView{ 1.0f };

        float m_VerticalFOV = 45.0f;
        float m_NearClip = 0.1f;
        float m_FarClip = 100.0f;

        glm::vec3 m_position{ 0.0f, 0.0f, 10.0f };
        glm::vec3 m_forwardDirection{ 0.0f, 0.0f, -1.0f };
        glm::quat m_rotation{ 1.0f, 0.0f, 0.0f, 0.0f };

        // Cached ray directions
        std::vector<glm::vec3> m_rayDirections;

        glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };

        uint64_t m_ViewportWidth = 0;
        uint64_t m_ViewportHeight = 0;

        float m_speed = 2.0f;
        float m_deltaSpeed = 0.2f;
        float m_maxSpeed = 30.0f;
    };
}