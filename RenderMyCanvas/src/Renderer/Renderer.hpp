#pragma once
#include "Utils/Utils.hpp"
#include "Walnut/Image.h"
#include "Scene/Scene.hpp"
#include "Camera/Camera.hpp"
#include "HityPayload.hpp"
#include "Ray.hpp"

namespace RMC::renderer {
    class Renderer {
    public:
        explicit Renderer() {};
        ~Renderer() = default;
        // @brief Resize {m_FinalImage}, {m_ImageData} and several other members based on the viewport size.
        //        [Note] Call this function when the viewport is resized or the final image is null.
        void OnResize(uint64_t width, uint64_t height);
        // @brief Render the scene, the result is stored in {m_FinalImage}.
        void Render(Ptr<scene::Scene> scene, Ptr<camera::Camera> camera);

        bool OnUpdate(float dt);
        std::shared_ptr<Walnut::Image> GetFinalImage() { return m_finalImage; }
        Ref<std::vector<std::shared_ptr<primitive::BasePrimitive>>> GetPrimitivePerPixel() { return m_entityPerPixel; }
        Ref<std::vector<std::shared_ptr<primitive::BasePrimitive>>> GetBufferPrimitivePerPixel() { return m_buffPrimitivePerPixel; }
        Range2F32 GetViewportSize() const { return { float(m_imageWidth), float(m_imageHeight) }; }
        std::vector<uint32_t>& GetBufferImage() { return m_imageBuffer; }
        const std::vector<uint32_t>& GetBufferImage() const { return m_imageBuffer; }

        glm::vec4 PerPixelColor(uint64_t x, uint64_t y);
        HitPayload TraceRay(const Ray& ray) const;

        HitPayload Miss(const Ray& ray) const
        {
            HitPayload payload;
            payload.hitDistance = -1.0f;
            return payload;
        }

        HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex) const;
        HitPayload ClosestHitPolygon(const Ray& ray, float hitDistance, int objectIndex) const;

        void ResetFrameIndex() { m_frameIndex = 1; }

        bool m_accumulate = true;
        Ptr<camera::Camera>                              m_activeCamera = nullptr;
        Ptr<scene::Scene>                                m_activeScene = nullptr;

    public:
        std::shared_ptr<Walnut::Image>                   m_finalImage{};
        std::vector<uint32_t>                              m_imageBuffer{};
        std::vector<std::shared_ptr<primitive::BasePrimitive>> m_entityPerPixel{};
        std::vector<float>                             m_zBuffer{};
        std::vector<std::shared_ptr<primitive::BasePrimitive>> m_buffPrimitivePerPixel{};
        std::vector<uint32_t>                              m_idMap{};
        uint64_t                                           m_imageWidth = 0;
        uint64_t                                           m_imageHeight = 0;

        std::vector<glm::vec4>                             m_accumulationData{};
        std::vector<uint64_t> m_imageHorizontalIter{}, m_imageVerticalIter{};

        uint32_t m_frameIndex = 1;
    };
} // namespace RMC::renderer