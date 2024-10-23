#include "pch.hpp"
#include "Layer.hpp"

#include "Primitive/Sphere.hpp"
#include "Primitive/Polygon.hpp"

#include "Algorithm/BoundaryFill.hpp"

#include "Component/DrawAlgo2D.hpp"
#include "Component/Artist.hpp"
#include "Component/Transform.hpp"
#include "Component/Material.hpp"

namespace RMC::layer {
    Layer::Layer()
    {
        m_renderer.m_activeCamera = &m_camera;
        m_renderer.m_activeScene = &m_scene;
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.85f, 0.95f, 1.0f, 1.0f); // 窗口背景颜色
        style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // 文本颜色
        style.Colors[ImGuiCol_Button] = ImVec4(0.7f, 0.9f, 1.0f, 1.0f); // 按钮颜色
        style.Colors[ImGuiCol_Border] = ImVec4(0.8f, 0.9f, 1.0f, 1.0f); // 边框颜色
        style.Colors[ImGuiCol_Separator] = ImVec4(0.7f, 0.9f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.53f, 0.81f, 0.92f, 1.0f); 
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.7f, 0.9f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.7f, 0.9f, 1.0f, 1.0f); // 下拉框背景颜色
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.9f, 0.9f, 1.0f, 1.0f); // 下拉菜单背景颜色
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.9f, 0.9f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.294f, 0.0f, 0.51f, 1.0f);

        //component::Material gray{};
        //gray.Albedo = { 0.5f,0.5f,0.5f,1.0f };
        //gray.Roughness = 0.1f;

        //component::Material white{};
        //white.Albedo = { 1.0f,1.0f,1.0f,1.0f };
        //white.Roughness = 1.0f;

        //component::Material pink{};
        //pink.Albedo = { 1.0f,0.0f,1.0f,1.0f };
        //pink.Roughness = 0.0f;
        //////pink.EmissionColor = pink.Albedo;
        //////pink.EmissionPower = 1.0f;

        //component::Material blueSphere{};
        //blueSphere.Albedo = { 100 / 255.f, 243 / 255.f, 1.0f,1.0f };
        //blueSphere.Roughness = 0.0f;
        //////blueSphere.EmissionColor = blueSphere.Albedo;
        //////blueSphere.EmissionPower = 0.5f;

        //component::Material redSun{};
        //redSun.Albedo = { 1.0f,0.0f,0.0f,1.0f };
        //redSun.Roughness = 0.1f;
        //redSun.EmissionColor = redSun.Albedo;
        //redSun.EmissionPower = 1.0f;

        //component::Material lightSphere{};
        //lightSphere.Albedo = { 253 / 255.f, 240 / 255.f, 170 / 255.f,1.0f };
        //lightSphere.Roughness = 1.0f;
        //lightSphere.EmissionColor = { 1.0f,1.0f,1.0f };
        //lightSphere.EmissionPower = 20.0f;

        //component::Material brownStar;
        //brownStar.Albedo = { 157 / 255.,88 / 255.,27 / 255., 1. };
        //brownStar.Roughness = 1.0f;

        /*{
            auto sphere = CreatePrimitive<primitive::Sphere>();
            auto trsf = sphere->GetComponent<component::Transform>();
            trsf->translate = { 0.0f, 1.0f, 0.0f };
            auto vlist = sphere->GetComponent<component::VertexList>();
            auto material = sphere->GetComponent<component::Material>();
            *material = gray;
            m_scene.test_spheres.emplace_back(sphere);
            m_scene.test_transforms.emplace_back(trsf);
            m_scene.test_materials.emplace_back(material);
            m_scene.test_vlists.emplace_back(vlist);
        }

        {
            auto sphere = CreatePrimitive<primitive::Sphere>();
            auto trsf = sphere->GetComponent<component::Transform>();
            trsf->translate = { 50.0f, 0.0f, -120.0f };
            auto material = sphere->GetComponent<component::Material>();
            *material = lightSphere;
            m_scene.test_spheres.emplace_back(sphere);
            m_scene.test_transforms.emplace_back(trsf);
            m_scene.test_materials.emplace_back(material);
            auto vlist = sphere->GetComponent<component::VertexList>();
            vlist->at(0).position = { 50.0f, 0.0f, 0.0f };
            vlist->at(1).position = { -50.0f, 0.0f, 0.0f };
            vlist->at(2).position = { 0.0f, 50.0f, 0.0f };
            vlist->at(3).position = { 0.0f, -50.0f, 0.0f };
            vlist->at(4).position = { 0.0f, 0.0f, 50.0f };
            vlist->at(5).position = { 0.0f, 0.0f, -50.0f };
            m_scene.test_vlists.emplace_back(vlist);
        }

        {
            auto sphere = CreatePrimitive<primitive::Sphere>();
            auto trsf = sphere->GetComponent<component::Transform>();
            trsf->translate = { 0.0f, -21.0f, 0.0f };
            auto material = sphere->GetComponent<component::Material>();
            auto vlist = sphere->GetComponent<component::VertexList>();
            vlist->at(0).position = { 20.0f, 0.0f, 0.0f };
            vlist->at(1).position = { -20.0f, 0.0f, 0.0f };
            vlist->at(2).position = { 0.0f, 20.0f, 0.0f };
            vlist->at(3).position = { 0.0f, -20.0f, 0.0f };
            vlist->at(4).position = { 0.0f, 0.0f, 20.0f };
            vlist->at(5).position = { 0.0f, 0.0f, -20.0f };
            *material = blueSphere;
            m_scene.test_spheres.emplace_back(sphere);
            m_scene.test_transforms.emplace_back(trsf);
            m_scene.test_materials.emplace_back(material);
            m_scene.test_vlists.emplace_back(vlist);
        }

        {
            auto sphere = CreatePrimitive<primitive::Sphere>();
            auto trsf = sphere->GetComponent<component::Transform>();
            trsf->translate = { -2,0,0 };
            auto material = sphere->GetComponent<component::Material>();
            auto vlist = sphere->GetComponent<component::VertexList>();
            *material = redSun;
            m_scene.test_spheres.emplace_back(sphere);
            m_scene.test_transforms.emplace_back(trsf);
            m_scene.test_materials.emplace_back(material);
            m_scene.test_vlists.emplace_back(vlist);
        }

        {
            auto sphere = CreatePrimitive<primitive::Sphere>();
            auto trsf = sphere->GetComponent<component::Transform>();
            trsf->translate = { 0,0,-50 };
            auto material = sphere->GetComponent<component::Material>();
            auto vlist = sphere->GetComponent<component::VertexList>();
            *material = brownStar;
            m_scene.test_spheres.emplace_back(sphere);
            m_scene.test_transforms.emplace_back(trsf);
            m_scene.test_materials.emplace_back(material);
            m_scene.test_vlists.emplace_back(vlist);
        }

        {
            auto polygon = CreatePrimitive<primitive::Polygon>();
            auto trsf = polygon->GetComponent<component::Transform>();
            auto vlist = polygon->GetComponent<component::VertexList>();
            vlist->resize(3);
            vlist->at(0).position = { 0.0f, 0.0f, 0.0f };
            vlist->at(1).position = { 1,0,0 };
            vlist->at(2).position = { 0.5,0,0.8663 };

            vlist->at(0).normal = { 0.0f,-1.0f,0.0f };
            trsf->translate = { 5.0f,1.0f,1.0f };
            auto material = polygon->GetComponent<component::Material>();
            *material = white;
            m_scene.test_polygons.emplace_back(polygon);
            m_scene.test_transforms_p.emplace_back(trsf);
            m_scene.test_materials_p.emplace_back(material);
            m_scene.test_vlists_p.emplace_back(vlist);
        }

        {
            auto polygon = CreatePrimitive<primitive::Polygon>();
            auto trsf = polygon->GetComponent<component::Transform>();
            auto vlist = polygon->GetComponent<component::VertexList>();
            vlist->resize(3);
            vlist->at(0).position = { 0.0f, 0.0f, 0.0f };
            vlist->at(1).position = { 1,0,0 };
            vlist->at(2).position = { 0.5,0.8165,0.2887};

            vlist->at(0).normal = glm::vec3(0.5,0.125,0.0722) - glm::vec3(0.5,0,0.2887);
            trsf->translate = { 5.0f,1.0f,1.0f };
            auto material = polygon->GetComponent<component::Material>();
            *material = white;
            m_scene.test_polygons.emplace_back(polygon);
            m_scene.test_transforms_p.emplace_back(trsf);
            m_scene.test_materials_p.emplace_back(material);
            m_scene.test_vlists_p.emplace_back(vlist);
        }

        {
            auto polygon = CreatePrimitive<primitive::Polygon>();
            auto trsf = polygon->GetComponent<component::Transform>();
            auto vlist = polygon->GetComponent<component::VertexList>();
            vlist->resize(3);
            vlist->at(0).position = { 0.0f, 0.0f, 0.0f };
            vlist->at(1).position = { 0.5,0,0.8660 };
            vlist->at(2).position = { 0.5,0.8165,0.2887};

            glm::vec3 n = glm::cross(vlist->at(1).position - vlist->at(0).position, vlist->at(2).position - vlist->at(0).position);

            glm::vec3 outside{ 0.5,0,0.2887 };
            if (glm::dot(n, outside - vlist->at(0).position) > 0) {
                n *= -1;
            }

            vlist->at(0).normal = n;
            trsf->translate = { 5.0f,1.0f,1.0f };
            auto material = polygon->GetComponent<component::Material>();
            *material = white;
            m_scene.test_polygons.emplace_back(polygon);
            m_scene.test_transforms_p.emplace_back(trsf);
            m_scene.test_materials_p.emplace_back(material);
            m_scene.test_vlists_p.emplace_back(vlist);
        }

        {
            auto polygon = CreatePrimitive<primitive::Polygon>();
            auto trsf = polygon->GetComponent<component::Transform>();
            auto vlist = polygon->GetComponent<component::VertexList>();
            vlist->resize(3);
            vlist->at(0).position = { 1,0,0 };
            vlist->at(1).position = { 0.5,0,0.8660 };
            vlist->at(2).position = { 0.5,0.8165,0.2887};

            glm::vec3 n = glm::cross(vlist->at(1).position - vlist->at(0).position, vlist->at(2).position - vlist->at(0).position);

            glm::vec3 outside{ 0.5,0,0.2887 };
            if (glm::dot(n, outside - vlist->at(0).position) > 0) {
                n *= -1;
            }

            vlist->at(0).normal = n;
            trsf->translate = { 5.0f,1.0f,1.0f };
            auto material = polygon->GetComponent<component::Material>();
            *material = white;
            m_scene.test_polygons.emplace_back(polygon);
            m_scene.test_transforms_p.emplace_back(trsf);
            m_scene.test_materials_p.emplace_back(material);
            m_scene.test_vlists_p.emplace_back(vlist);
        }*/

    }

    void Layer::OnUpdate(float dt)
    {
        if (m_camera.OnUpdate(dt)) {
            m_renderer.ResetFrameIndex();
        }
    }

    void Layer::OnUIRender()
    {
        Walnut::Timer timer;

        // Viewport for image rendering
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::Begin("Viewport");
        ImGui::PopStyleColor();
        m_fullScreenMouseCoord = ImGui::GetMousePos();
        glm::vec2 viewportMouseCoord = this->GetViewportMouseCoord();
        glm::vec3 worldMouseCoord = m_camera.ScreenToWorld(this->GetViewportMouseCoord());
        this->UpdateViewportRange();
        this->UpdateViewportBounds();
        this->OutputFinalImage();
        ImGui::End();
        ImGui::PopStyleVar();


        // Status
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::Begin("Status");
        ImGui::PopStyleColor();
        ImGui::Text(
            "Mouse coord: (%d, %d)",
            static_cast<int32_t>(viewportMouseCoord.x),
            static_cast<int32_t>(viewportMouseCoord.y)
        );
        ImGui::Text(
            "Mouse World Position: (%.2f, %.2f, %.2f)",
            worldMouseCoord.x, worldMouseCoord.y, worldMouseCoord.z
        );
        ImGui::Separator();
        /*ImGui::Text(
            "Camera World Position: (%.2f, %.2f, %.2f)",
            m_camera.GetPosition().x, m_camera.GetPosition().y, m_camera.GetPosition().z
        );
        ImGui::Text(
            "Camera World Forward Direction: (%.2f, %.2f, %.2f)",
            m_camera.GetForwardDirection().x,
            m_camera.GetForwardDirection().y,
            m_camera.GetForwardDirection().z
        );*/
        ImGui::Text(
            "Camera Speed: %.2f",
            m_camera.GetSpeed()
        );
        ImGui::Separator();
        this->Block_PrimitiveUnderMouse();
        ImGui::Separator();
        this->Block_ComponentControlPanel();
        ImGui::Separator();

        ImGui::End();

        // Configurations
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::Begin("Configuration");
        ImGui::PopStyleColor();
        ImGui::Text("Last Render Time: %.2f", m_lastRenderTime);
        ImGui::SameLine();
        ImGui::Separator();
        /*ImGui::Checkbox("RayTrace", &m_scene.isRayTracing);

        ImGui::Checkbox("Accumulate", &m_renderer.m_accumulate);

        if (ImGui::Button("Reset"))
            m_renderer.ResetFrameIndex();*/

        static int selected_item = 0;
        static const char* items[] = { "Stop", "Render" };

        ImGui::ListBox(" ", &selected_item, items, IM_ARRAYSIZE(items), 2);
        switch (selected_item)
        {
        case 0: { m_settings.renderStop = true; break; }
        case 1: { m_settings.renderStop = false; break; }
        }
        ImGui::Separator();
        static float color_1[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        ImGui::ColorEdit4("Color 1", color_1);
        m_globalMaterial_1.Albedo = { color_1[0], color_1[1], color_1[2], color_1[3] };
        ImGui::Separator();
        static float color_2[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        ImGui::ColorEdit4("Color 2", color_2);
        m_globalMaterial_2.Albedo = { color_2[0], color_2[1], color_2[2], color_2[3] };

        ImGui::End();

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::Begin("Drawing Tools");
        ImGui::PopStyleColor();
        if (ImGui::Button("Line Segment")) { m_settings.curDrawing = Settings::CUR_DRAWING::LINE_SEGMENT; m_settings.drawStep = 0; }
        ImGui::SameLine();
        if (ImGui::Button("Ellipse")) { m_settings.curDrawing = Settings::CUR_DRAWING::ELLIPSE; m_settings.drawStep = 0; }
        ImGui::SameLine();
        if (ImGui::Button("Arc")) { m_settings.curDrawing = Settings::CUR_DRAWING::ARC; m_settings.drawStep = 0; }
        ImGui::SameLine();
        if (ImGui::Button("Polygon")) { m_settings.curDrawing = Settings::CUR_DRAWING::POLYGON; m_settings.drawStep = 0; }
        ImGui::SameLine();
        if (ImGui::Button("Curve")) { m_settings.curDrawing = Settings::CUR_DRAWING::CURVE; m_settings.drawStep = 0; }
        ImGui::SameLine();
        if (ImGui::Button("Parent")) { m_settings.curDrawing = Settings::CUR_DRAWING::PARENT; m_settings.drawStep = 0; }
        ImGui::SameLine();
        if (ImGui::Button("Seed Fill")) { m_settings.curDrawing = Settings::CUR_DRAWING::SEED_FILL; m_settings.drawStep = 0; }
        ImGui::End();

        if (!m_settings.renderStop) {
            switch (m_settings.curDrawing)
            {
            case Settings::CUR_DRAWING::LINE_SEGMENT: { Interact_LineSegment(); break; }
            case Settings::CUR_DRAWING::ELLIPSE: { Interact_Ellipse(); break; }
            case Settings::CUR_DRAWING::ARC: { Interact_Arc(); break; }
            case Settings::CUR_DRAWING::POLYGON: { Interact_Polygon(); break; }
            case Settings::CUR_DRAWING::CURVE: { Interact_Curve(); break; }
            case Settings::CUR_DRAWING::PARENT: { Interact_Parent(); break; }
            }
        }


        if (!m_settings.renderStop) {
            Render();
        }
        if (m_settings.renderStop && m_settings.curDrawing == Settings::CUR_DRAWING::SEED_FILL) {
            if (Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Left)) {
                glm::vec2 screenMouseCoord = GetViewportMouseCoord();
                algorithm::BoundaryFill filler{};
                filler.SetParams(
                    glm::u64vec2{ screenMouseCoord },
                    &m_renderer.GetBufferImage(),
                    m_viewportWidth,
                    m_viewportHeight,
                    utils::RGBA_to_uint32_t(m_globalMaterial_1.Albedo)
                );
                filler.Draw();
                m_renderer.GetFinalImage()->SetData(m_renderer.GetBufferImage().data());
                m_settings.curDrawing = Settings::CUR_DRAWING::NONE_SHAPE;
            }
        }

        m_lastRenderTime = timer.ElapsedMillis();
    }

    void Layer::Render()
    {
        auto [curWidth, curHeight] = m_renderer.GetViewportSize();
        if (!m_renderer.GetFinalImage() || curWidth != m_viewportWidth || curHeight != m_viewportHeight) {
            // Renderer and camera must have the same view port size
            m_renderer.OnResize(m_viewportWidth, m_viewportHeight);
            m_camera.OnResize(m_viewportWidth, m_viewportHeight);
            m_camera.OnResize(m_viewportWidth, m_viewportHeight);
        }
        m_renderer.Render(&m_scene, &m_camera);
    }

    glm::vec2 Layer::GetViewportMouseCoord() const
    {
        glm::vec2 result{};
        float x = m_fullScreenMouseCoord.x - m_viewportBounds[0].x;
        float y = m_viewportBounds[1].y - m_fullScreenMouseCoord.y;
        return {
            x < static_cast<float>(m_viewportWidth) ? x : -1.0,
            y < static_cast<float>(m_viewportHeight) ? y : -1.0
        };
    }

    void Layer::UpdateViewportBounds()
    {
        auto minRegion = ImGui::GetWindowContentRegionMin();
        auto maxRegion = ImGui::GetWindowContentRegionMax();
        auto offset = ImGui::GetWindowPos();

        this->m_viewportBounds[0] = {
            minRegion.x + offset.x,
            minRegion.y + offset.y
        };

        this->m_viewportBounds[1] = {
            maxRegion.x + offset.x,
            maxRegion.y + offset.y
        };
    }

    void Layer::UpdateViewportRange()
    {
        this->m_viewportWidth = uint32_t(ImGui::GetContentRegionAvail().x);
        this->m_viewportHeight = uint32_t(ImGui::GetContentRegionAvail().y);
    }

    void Layer::OutputFinalImage()
    {
        auto image = m_renderer.GetFinalImage();
        if (image != nullptr) {
            ImGui::Image(
                image->GetDescriptorSet(),
                { (float)image->GetWidth(), (float)image->GetHeight() },
                ImVec2(0, 1), ImVec2(1, 0)
            );
        }
    }

    void Layer::Create_Portal()
    {
        static int createStep = 0;

        component::Material greenPortal;
        greenPortal.Albedo = { 0.0f,1.0f,0.0f,1.0f };
        greenPortal.EmissionColor = greenPortal.Albedo;
        greenPortal.EmissionPower = 10.0f;
        glm::vec3 cameraDirection = m_camera.GetForwardDirection();

        switch (createStep) {

        case 0: {
            if (Walnut::Input::IsKeyDown(Walnut::Key::T)) {
                createStep = 1;
            }
            break;
        }
        case 1: {
            std::array<glm::vec3, 4> portalPoints{};

            if (!Walnut::Input::IsKeyDown(Walnut::Key::T)) {
                portalPoints[0] = m_camera.ScreenToWorld({ m_renderer.m_imageWidth / 2 - 40, m_renderer.m_imageHeight / 2 - 80 });
                portalPoints[1] = m_camera.ScreenToWorld({ m_renderer.m_imageWidth / 2 - 40, m_renderer.m_imageHeight / 2 + 80 });
                portalPoints[2] = m_camera.ScreenToWorld({ m_renderer.m_imageWidth / 2 + 40, m_renderer.m_imageHeight / 2 + 80 });
                portalPoints[3] = m_camera.ScreenToWorld({ m_renderer.m_imageWidth / 2 + 40, m_renderer.m_imageHeight / 2 - 80 });

                {
                    auto polygon = CreatePrimitive<primitive::Polygon>();
                    auto trsf = polygon->GetComponent<component::Transform>();
                    auto vlist = polygon->GetComponent<component::VertexList>();
                    vlist->resize(3);

                    vlist->at(0).position = portalPoints[0];
                    vlist->at(1).position = portalPoints[1];
                    vlist->at(2).position = portalPoints[2];

                    vlist->at(0).normal = { cameraDirection[0], cameraDirection[1], -cameraDirection[2] };

                    auto material = polygon->GetComponent<component::Material>();

                    *material = greenPortal;

                    m_scene.test_polygons.emplace_back(polygon);
                    m_scene.test_transforms_p.emplace_back(trsf);
                    m_scene.test_materials_p.emplace_back(material);
                    m_scene.test_vlists_p.emplace_back(vlist);
                }
                {
                    auto polygon = CreatePrimitive<primitive::Polygon>();
                    auto trsf = polygon->GetComponent<component::Transform>();
                    auto vlist = polygon->GetComponent<component::VertexList>();
                    vlist->resize(3);

                    vlist->at(0).position = portalPoints[2];
                    vlist->at(1).position = portalPoints[3];
                    vlist->at(2).position = portalPoints[0];

                    vlist->at(0).normal = { cameraDirection[0], cameraDirection[1], cameraDirection[2] };

                    auto material = polygon->GetComponent<component::Material>();

                    *material = greenPortal;

                    m_scene.test_polygons.emplace_back(polygon);
                    m_scene.test_transforms_p.emplace_back(trsf);
                    m_scene.test_materials_p.emplace_back(material);
                    m_scene.test_vlists_p.emplace_back(vlist);
                }

                ////{
                ////    auto polygon = CreatePrimitive<primitive::Polygon>();
                ////    auto trsf = polygon->GetComponent<component::Transform>();
                ////    auto vlist = polygon->GetComponent<component::VertexList>();
                ////    vlist->resize(3);

                ////    vlist->at(0).position = m_camera.ScreenToWorld({ m_renderer.m_imageWidth / 2 + 40, m_renderer.m_imageHeight / 2 + 80 });
                ////    vlist->at(1).position = m_camera.ScreenToWorld({ m_renderer.m_imageWidth / 2 + 40, m_renderer.m_imageHeight / 2 - 80 });
                ////    vlist->at(2).position = m_camera.ScreenToWorld({ m_renderer.m_imageWidth / 2 - 40, m_renderer.m_imageHeight / 2 + 80 });

                ////    vlist->at(0).normal = { cameraDirection[0], cameraDirection[1], -cameraDirection[2] };

                ////    auto material = polygon->GetComponent<component::Material>();

                ////    *material = greenPortal;

                ////    m_scene.test_polygons.emplace_back(polygon);
                ////    m_scene.test_transforms_p.emplace_back(trsf);
                ////    m_scene.test_materials_p.emplace_back(material);
                ////    m_scene.test_vlists_p.emplace_back(vlist);


                ////}

                ////{
                ////    auto polygon = CreatePrimitive<primitive::Polygon>();
                ////    auto trsf = polygon->GetComponent<component::Transform>();
                ////    auto vlist = polygon->GetComponent<component::VertexList>();
                ////    vlist->resize(3);

                ////    vlist->at(0).position = m_camera.ScreenToWorld({ m_renderer.m_imageWidth / 2 + 40, m_renderer.m_imageHeight / 2 + 80 });
                ////    vlist->at(1).position = m_camera.ScreenToWorld({ m_renderer.m_imageWidth / 2 + 40, m_renderer.m_imageHeight / 2 - 80 });
                ////    vlist->at(2).position = m_camera.ScreenToWorld({ m_renderer.m_imageWidth / 2 - 40, m_renderer.m_imageHeight / 2 + 80 });

                ////    vlist->at(0).normal = { cameraDirection[0], cameraDirection[1], cameraDirection[2] };

                ////    auto material = polygon->GetComponent<component::Material>();

                ////    *material = greenPortal;

                ////    m_scene.test_polygons.emplace_back(polygon);
                ////    m_scene.test_transforms_p.emplace_back(trsf);
                ////    m_scene.test_materials_p.emplace_back(material);
                ////    m_scene.test_vlists_p.emplace_back(vlist);


                ////}

                /*if (m_scene.firstPortal.empty()) {
                    std::cout << "First Set";
                    m_scene.firstPortal = portalPoints;
                } else {
                    m_scene.secondPortal = portalPoints;
                }*/

                std::cout << "Create Finished" << std::endl;
                createStep = 0;
            }
            break;
        }
        }
    }

    bool isPointInRectangle(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C, const glm::vec3& D, const glm::vec3& P) 
    {
        glm::vec3 AC = C - A;
        glm::vec3 BD = D - B;
        glm::vec3 O = (A + C) / 2.0f;

        glm::vec3 OP = P - O;
        float alpha = glm::acos(glm::dot(OP, AC) / (glm::length(OP) * glm::length(AC)));
        float beta = glm::acos(glm::dot(OP, BD) / (glm::length(OP) * glm::length(BD)));

        return alpha < glm::pi<float>() / 2.0f && beta < glm::pi<float>() / 2.0f;
    }

    /*void Layer::Teleport()
    {
        if (m_scene.firstPortal.empty() || m_scene.secondPortal.empty()) {
            return;
        }

        glm::vec3 fisrtPortalCenter = 
            (m_scene.firstPortal[0] + m_scene.firstPortal[1] + m_scene.firstPortal[2] + m_scene.firstPortal[3]) / 4.f;
        glm::vec3 secondPortalCenter = 
            (m_scene.secondPortal[0] + m_scene.secondPortal[1] + m_scene.secondPortal[2] + m_scene.secondPortal[3]) / 4.f;
        if (glm::distance(m_camera.GetPosition(), fisrtPortalCenter) < 0.5) {
            m_camera.SetPosition(secondPortalCenter + m_camera.GetForwardDirection() * 1.f);
        } else if (glm::distance(m_camera.GetPosition(), secondPortalCenter) < 0.5) {
            m_camera.SetPosition(fisrtPortalCenter+ m_camera.GetForwardDirection() * 1.f);
        }
    }*/

}