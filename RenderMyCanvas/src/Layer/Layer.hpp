#pragma once

#include "Walnut/Application.h"
#include "Walnut/Layer.h"
#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Walnut/Input/Input.h"

#include "Utils/Utils.hpp"
#include "Utils/Concepts.hpp"
#include "Utils/Log.hpp"

#include "Renderer/Renderer.hpp"

#include "Camera/Camera.hpp"

#include "Component/Transform.hpp"
#include "Component/Artist.hpp"
#include "Component/VertexList.hpp"
#include "Component/DrawAlgo2D.hpp"
#include "Component/OptionMap.hpp"

#include "Scene/SceneSerializer.hpp"
#include "Scene/Scene.hpp"
namespace RMC::layer {
    class Layer : public Walnut::Layer
    {
        struct Settings
        {
            enum class CUR_DRAWING : int8_t
            {
                NONE_SHAPE = 0,
                LINE_SEGMENT,
                ELLIPSE,
                POLYGON,
                CURVE,
                ARC,
                BOUNDARY_FILL,
                SEED_FILL,
                PARENT,
            } curDrawing = CUR_DRAWING::NONE_SHAPE;

            uint32_t drawStep = 0;
            uint32_t selectStep = 0;
            uint32_t childSelectStep = 0;
            bool   renderStop = true;
        };

    public:
        explicit Layer();

    public:
        virtual void OnUpdate(float dt) override;
        virtual void OnUIRender() override;
        void Render();

    private:
        glm::vec2 GetViewportMouseCoord() const;
        void UpdateViewportBounds();
        void UpdateViewportRange();
        void OutputFinalImage();

        void Interact_Ellipse();
        void Interact_LineSegment();
        void Interact_Polygon();
        void Interact_Curve();
        void Interact_Arc();
        void Interact_Parent();

        void Block_PrimitiveUnderMouse();
        void Block_ComponentControlPanel();

        void Create_Portal();
        void Teleport();

    public:
        template<typename Primitive>
            requires std::is_base_of_v<primitive::BasePrimitive, Primitive>
        auto CreatePrimitive() -> std::shared_ptr<primitive::BasePrimitive>;

        template<typename Primitive>
            requires std::is_base_of_v<primitive::BasePrimitive, Primitive>
        void RegisterPrimitive(String name)
        {
            m_scene.entityFactory.Register(
                name,
                [this]() -> std::shared_ptr<primitive::BasePrimitive> {
                    return this->CreatePrimitive<Primitive>();
                }
            );
        }

        template<typename Point>
        bool IsInViewport(const Point& point)
        {
            return utils::IsInRange(point[0], 0, m_viewportWidth) && utils::IsInRange(point[1], 0, m_viewportHeight);
        }

    private:
        scene::Scene            m_scene{};
        renderer::Renderer      m_renderer{};

        Settings                m_settings;

        uint32_t                  m_viewportWidth = 0;
        uint32_t                  m_viewportHeight = 0;
        ImVec2                  m_viewportBounds[2] = { {0.0f, 0.0f}, {0.0f, 0.0f} };

        camera::Camera          m_camera{ 45.F,0.1F,100.F };

        ImVec2                  m_fullScreenMouseCoord = { 0.0f, 0.0f };
        float                 m_lastRenderTime = 0.0f;
        component::Material     m_globalMaterial_1{};
        component::Material     m_globalMaterial_2{};

        std::shared_ptr<primitive::BasePrimitive> m_selectedPrimitive = nullptr; // The primitive under mouse
        std::shared_ptr<primitive::BasePrimitive> m_lastSelectedPrimitive = nullptr; // The primitive under mouse
    };
} // namespace RMC::layer