#include "pch.hpp"
#include "Camera.hpp"
#include "Walnut/Input/Input.h"

namespace RMC::camera {
    Camera::Camera(float verticalFOV, float nearClip, float farClip)
        : m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
    {
        m_rotation = glm::rotation({ 0.0, 0.0, -1.0 }, m_forwardDirection);
    }

    bool Camera::OnUpdate(float dt)
    {
        glm::vec2 mousePos = Walnut::Input::GetMousePosition();
        glm::vec2 delta = (mousePos - m_LastMousePosition) * 0.003f;
        m_LastMousePosition = mousePos;

        if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Right))
        {
            Walnut::Input::SetCursorMode(Walnut::CursorMode::Normal);
            return false;
        }

        Walnut::Input::SetCursorMode(Walnut::CursorMode::Locked);

        bool moved = false;

        constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
        glm::vec3 rightDirection = glm::cross(m_forwardDirection, upDirection);


        // Movement
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W)) {
            m_position += m_forwardDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        } else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S)) {
            m_position -= m_forwardDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        }else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A)) {
            m_position -= rightDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        } else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D)) {
            m_position += rightDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        } else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Q)) {
            m_position -= upDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        } else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::E)) {
            m_position += upDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        } else {
            m_speed = 2.0f;
        }

        // Rotation
        if (delta.x != 0.0f || delta.y != 0.0f) {
            float pitchDelta = delta.y * GetRotationSpeed();
            float yawDelta = delta.x * GetRotationSpeed();

            glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
                glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
            m_forwardDirection = glm::rotate(q, m_forwardDirection);
            m_rotation = glm::rotation({ 0.0, 0.0, -1.0 }, m_forwardDirection);

            moved = true;
        }

        if (moved) {
            RecalculateView();
            RecalculateRayDirections();
        }

        return moved;
    }

    void Camera::OnResize(uint32_t width, uint32_t height)
    {
        if (width == m_ViewportWidth && height == m_ViewportHeight)
            return;

        m_ViewportWidth = width;
        m_ViewportHeight = height;

        RecalculateProjection();
        RecalculateView();
        RecalculateRayDirections();
    }

    float Camera::GetRotationSpeed()
    {
        return 0.15f;
    }

    glm::vec2 Camera::WorldToScreen(Ref<const glm::vec3> point) const
    {
        glm::vec4 cameraSpacePoint = m_View * glm::vec4(point, 1.0f);
        glm::vec4 clipSpacePoint = m_Projection * cameraSpacePoint;
        if (clipSpacePoint.w <= 0.0F) { return glm::vec2(-1.0f, -1.0f); }
        glm::vec2 screenSpacePoint{};
        screenSpacePoint.x = (clipSpacePoint.x / clipSpacePoint.w + 1.0f) * 0.5f * m_ViewportWidth;
        screenSpacePoint.y = m_ViewportHeight - (1.0f - clipSpacePoint.y / clipSpacePoint.w) * 0.5f * m_ViewportHeight;
        return screenSpacePoint;
    }

    glm::vec3 Camera::ScreenToWorld(Ref<const glm::vec2> screenPoint, float forwardDistance) const
    {
        if (m_ViewportHeight == 0 || m_ViewportWidth == 0) return{ -1,-1,-1 };

        float normalizedX = (2.0f * screenPoint.x / m_ViewportWidth) - 1.0f;
        float normalizedY = 1.0f - (2.0f * (m_ViewportHeight - screenPoint.y) / m_ViewportHeight);
        glm::vec4 cameraSpacePoint(normalizedX, normalizedY, -forwardDistance, 1.0f);
        glm::vec4 worldSpacePoint = m_InverseProjection * cameraSpacePoint;
        worldSpacePoint.w = 1.0f;
        worldSpacePoint = m_InverseView * worldSpacePoint;
        return glm::vec3{ worldSpacePoint.x, worldSpacePoint.y, worldSpacePoint.z };
    }

    void Camera::RecalculateProjection()
    {
        m_Projection = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_NearClip, m_FarClip);
        m_InverseProjection = glm::inverse(m_Projection);
    }

    void Camera::RecalculateView()
    {
        m_View = glm::lookAt(m_position, m_position + m_forwardDirection, glm::vec3(0, 1, 0));
        m_InverseView = glm::inverse(m_View);
    }

    void Camera::RecalculateRayDirections()
    {
        m_rayDirections.resize(m_ViewportWidth * m_ViewportHeight);

        for (uint32_t y = 0; y < m_ViewportHeight; y++)
        {
            for (uint32_t x = 0; x < m_ViewportWidth; x++)
            {
                glm::vec2 coord = { (float)x / (float)m_ViewportWidth, (float)y / (float)m_ViewportHeight };
                coord = coord * 2.0f - 1.0f; // -1 -> 1

                glm::vec4 target = m_InverseProjection * glm::vec4(coord.x, coord.y, -1, 1);
                glm::vec3 rayDirection = glm::vec3(m_InverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
                m_rayDirections[x + y * m_ViewportWidth] = rayDirection;
            }
        }
    }
} // namespace RMC::camera