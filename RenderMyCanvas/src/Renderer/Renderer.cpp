#include "pch.hpp"

#include "Renderer.hpp"

#include "Algorithm/ScanLineFill.hpp"
#include "Component/Artist.hpp"

#include "Component/Transform.hpp"
#include "Component/Material.hpp"
#include "Walnut/Random.h"

namespace RMC::renderer {
    void Renderer::OnResize(uint64_t width, uint64_t height)
    {
        if (m_finalImage) {
            m_finalImage->Resize(width, height);
        } else {
            m_finalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
        }
        m_imageBuffer.resize(width * height, 0);
        m_zBuffer.resize(width * height, std::numeric_limits<float>::max());
        m_accumulationData.resize(width * height, glm::vec4{ 0.0f });
        m_entityPerPixel.resize(width * height, nullptr);
        m_buffPrimitivePerPixel.resize(width * height, nullptr);
        m_imageWidth = width;
        m_imageHeight = height;

        m_imageHorizontalIter.resize(width);
        m_imageVerticalIter.resize(height);
        for (uint32_t i = 0; i < width; i++)
            m_imageHorizontalIter[i] = i;
        for (uint32_t i = 0; i < height; i++)
            m_imageVerticalIter[i] = i;
    }

    void Renderer::Render(Ptr<scene::Scene> activeScene, Ptr<camera::Camera> activeCamera)
    {
        if (!activeScene) { RMC_ERROR("Scene not found."); return; }
        if (!activeCamera) { RMC_ERROR("Camera not found."); return; }

        // Set image buffer to black
        std::ranges::fill(m_imageBuffer, 0);
        std::ranges::fill(m_buffPrimitivePerPixel, nullptr);

        if (m_frameIndex == 1) {
            std::ranges::fill(m_accumulationData, glm::vec4{ 0.0f });
        }
        std::for_each(std::execution::par, m_imageVerticalIter.begin(), m_imageVerticalIter.end(),
            [this](uint32_t y)
            {
                std::for_each(std::execution::par, m_imageHorizontalIter.begin(), m_imageHorizontalIter.end(),
                [this, y](uint32_t x)
                    {
                        glm::vec4 color = this->PerPixelColor(x, y);
                        m_accumulationData[x + y * m_imageWidth] += color;

                        glm::vec4 accumulatedColor = m_accumulationData[x + y * m_imageWidth];
                        accumulatedColor /= (float)m_frameIndex;

                        accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
                        m_imageBuffer[x + y * m_imageWidth] = utils::RGBA_to_uint32_t(accumulatedColor);
                    });
            });

        // ||Bug|| [15/Sep/2023]
        //   Multi-thread causes wrong result.
        std::for_each(
            ////std::execution::par,
            activeScene->entities.begin(),
            activeScene->entities.end(),
            [this, &activeCamera](std::shared_ptr<primitive::BasePrimitive> primitive) {
                primitive->Draw(activeCamera);
            }
        );
        std::for_each(
            activeScene->tempEntities.begin(),
            activeScene->tempEntities.end(),
            [this, &activeCamera](std::shared_ptr<primitive::BasePrimitive> primitive) {
                primitive->Draw(activeCamera);
            }
        );
        std::for_each(
            activeScene->controlPoints.begin(),
            activeScene->controlPoints.end(),
            [this, &activeCamera](std::shared_ptr<primitive::BasePrimitive> primitive) {
                primitive->Draw(activeCamera);
            }
        );
        
        // Pass image buffer to final image
        m_finalImage->SetData(m_imageBuffer.data());
        m_entityPerPixel = m_buffPrimitivePerPixel;

        if (m_accumulate) {
            m_frameIndex++;
        } else {
            m_frameIndex = 1;
        }
    }

    bool Renderer::OnUpdate(float dt)
    {
        return false;
    }

    glm::vec4 Renderer::PerPixelColor(uint64_t x, uint64_t y) 
    {
        Ray ray{};
        ray.origin = m_activeCamera->GetPosition();
        ray.direction = m_activeCamera->GetRayDirections()[x + y * m_imageWidth];

        uint32_t seed = x + y * m_imageWidth;
        seed *= m_frameIndex;

        glm::vec3 light{ 0.0f };
        glm::vec3 contribution{ 1.0f, 1.0f, 1.0f };
        glm::vec3 illumination{ 0.0f };

        int32_t bounces = 3;

        for (int32_t i = 0; i < bounces; ++i) {
            seed += i;

            renderer::HitPayload payload = TraceRay(ray);
            if (payload.hitDistance < 0.0f) {
                constexpr glm::vec3 skyColor = glm::vec3{ 10/255., 0/255., 60/255. };
                illumination += skyColor * contribution;
                break;
            }

            if (i == 0 && payload.hitDistance < m_zBuffer[x + y * m_imageWidth]) {
                ////m_zBuffer[x + y * m_imageWidth] = payload.hitDistance;
                ////m_buffPrimitivePerPixel[x + y * m_imageWidth] = m_activeScene->test_spheres[payload.objectIndex];
            }

            std::shared_ptr<primitive::BasePrimitive> hitPrimitive = nullptr;
            std::shared_ptr<component::Material> material = nullptr;
            if (payload.sphereOrPolygon == 0) {
                hitPrimitive = m_activeScene->test_spheres[payload.objectIndex];
                material = m_activeScene->test_materials[payload.objectIndex];
            } else {
                hitPrimitive = m_activeScene->test_polygons[payload.objectIndex];
                material = m_activeScene->test_materials_p[payload.objectIndex];
            }

            contribution *= glm::vec3{ material->Albedo };

            light += material->GetEmission();
            illumination += light * contribution;

            ray.origin = payload.worldPosition + payload.worldNormal * 0.0001f;

            ray.direction = glm::normalize(payload.worldNormal + 
                 glm::normalize(glm::vec3{
                    math::Randomfloat(seed) * 2.0f - 1.0f,
                    math::Randomfloat(seed) * 2.0f -1.0f,
                    math::Randomfloat(seed) * 2.0f -1.0f
                    })
                );

            ////ray.direction = glm::reflect(ray.direction, payload.worldNormal);
        }

        return glm::vec4{ illumination, 1.0f };
    }

    HitPayload Renderer::TraceRay(const Ray & ray) const
    {
        int closestSphere = -1; 
        int closestPolygon = -1;

        float hitDistance = std::numeric_limits<float>::max();
        for (size_t i = 0; i < m_activeScene->test_spheres.size(); i++)
        {
            const auto& sphere = m_activeScene->test_spheres[i];
            const auto& trsf = m_activeScene->test_transforms[i];
            const auto& vlist = m_activeScene->test_vlists[i];

            float closestT = sphere->Hit(ray, *trsf, *vlist);

            if (closestT > 0.0f && closestT < hitDistance)
            {
                hitDistance = closestT;
                closestSphere = (int)i;
            }

        }

        for (size_t i = 0; i < m_activeScene->test_polygons.size(); i++)
        {
            const auto& polygon = m_activeScene->test_polygons[i];
            const auto& trsf = m_activeScene->test_transforms_p[i];
            const auto& vlist = m_activeScene->test_vlists_p[i];

            float closestT = polygon->Hit(ray, *trsf, *vlist);

            if (closestT > 0.0f && closestT < hitDistance)
            {
                hitDistance = closestT;
                closestSphere = -1;
                closestPolygon = (int)i;
            }
        }

        if (closestSphere >= 0) {
            return ClosestHit(ray, hitDistance, closestSphere);
        } else if (closestPolygon >= 0) {
            return ClosestHitPolygon(ray, hitDistance, closestPolygon);
        } else {
            return Miss(ray);
        }
    }

    HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex) const
    {
        HitPayload payload;
        payload.hitDistance = hitDistance;
        payload.objectIndex = objectIndex;

        const auto& closestSphere = m_activeScene->test_spheres[objectIndex];
        const auto& trsf = m_activeScene->test_transforms[objectIndex];
        glm::vec3 sphereCenter = trsf->translate;

        glm::vec3 origin = ray.origin - sphereCenter;
        payload.worldPosition = origin + ray.direction * hitDistance;
        payload.worldNormal = glm::normalize(payload.worldPosition);

        payload.worldPosition += sphereCenter;

        return payload;
    }

    HitPayload Renderer::ClosestHitPolygon(const Ray& ray, float hitDistance, int objectIndex) const
    {
        HitPayload payload;
        payload.hitDistance = hitDistance;
        payload.objectIndex = objectIndex;
        payload.sphereOrPolygon = true;

        const auto& closestTriangle = m_activeScene->test_polygons[objectIndex];
        const auto& vlists = m_activeScene->test_vlists_p[objectIndex];

        payload.worldNormal = vlists->at(0).normal;
        payload.worldPosition = ray.origin + ray.direction * hitDistance;

        // Check if the angle is larger than 90
        if (glm::dot(payload.worldNormal, ray.direction) > 0.0f) {
            ////return Miss(ray);
        }

        return payload;
    }

} // namespace RMC::renderer