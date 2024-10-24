#include <execution>

#include "Renderer.h"
#include "Components/TransformComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MaterialComponent.h"

namespace Utils {
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint32_t r = (uint32_t)(color.r * 255.0f);
		uint32_t g = (uint32_t)(color.g * 255.0f);
		uint32_t b = (uint32_t)(color.b * 255.0f);
		uint32_t a = (uint32_t)(color.a * 255.0f);

		return (a << 24) | (b << 16) | (g << 8) | r;
	}

	static uint32_t PCG_Hash(uint32_t input) 
	{
        uint32_t state = input * 747796405u + 2891336453u;
        uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }

	static float RandomFloat(uint32_t &seed) 
	{
		seed = PCG_Hash(seed);
		return (float)seed / (float)std::numeric_limits<uint32_t>::max();
    }

	static glm::vec3 InUnitSphere(uint32_t& seed)
	{
		return glm::normalize(glm::vec3(
			RandomFloat(seed) * 2.0f - 1.0f,
			RandomFloat(seed) * 2.0f - 1.0f,
			RandomFloat(seed) * 2.0f - 1.0f)
		);
	}
} // namespace Utils

namespace RMC
{
		Renderer::Renderer()
	{
		m_ImageData = nullptr;
		m_PpPipeline = std::make_unique<PostProcessingPipeLine>();
		m_PpPipeline->addProcess(std::make_shared<DLSSProcess>());
	}
	void Renderer::OnResize(uint32_t width, uint32_t height)
	{
		//resize the image
		if (m_FinalImage)
		{
			if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
				return;

			m_FinalImage->Resize(width, height);
		}
		else
		{
			m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
		}

		delete[] m_ImageData;
		m_ImageData = new uint32_t[width * height];

		delete[] m_AccumulationData;
		m_AccumulationData = new glm::vec4[width * height];

		m_ImageHorizontalIter.resize(width);
		m_ImageVerticalIter.resize(height);
		for (uint32_t i = 0; i < width; i++)
			m_ImageHorizontalIter[i] = i;
		for (uint32_t i = 0; i < height; i++)
			m_ImageVerticalIter[i] = i;
	}

    void Renderer::Render(const Scene& scene, const Camera& camera)
    {
        m_ActiveScene = &scene;
        m_ActiveCamera = &camera;
        m_Registry = &scene.GetRegistry();

        if (m_FrameIndex == 1)
            memset(m_AccumulationData, 0, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(glm::vec4));

        std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
            [this](uint32_t y)
            {
                std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
                    [this, y](uint32_t x)
                    {
                        glm::vec4 color = PerPixel(x, y);
                        m_AccumulationData[x + y * m_FinalImage->GetWidth()] += color;
                        glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_FinalImage->GetWidth()];
                        accumulatedColor /= (float)m_FrameIndex;
                        accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
                        m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(accumulatedColor);
                    });
            });

        m_FinalImage->SetData(m_ImageData);

        if (m_Settings.Accumulate)
            m_FrameIndex++;
        else
            m_FrameIndex = 1;
    }

    // 修改PerPixel方法
    glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
    {
        Ray ray;
        ray.Origin = m_ActiveCamera->GetPosition();
        ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

        glm::vec3 light(0.0f);
        glm::vec3 contribution(1.0f);

        uint32_t seed = x + y * m_FinalImage->GetWidth();
        seed *= m_FrameIndex;

        int bounces = 5;
        for (int i = 0; i < bounces; i++)
        {
            seed += i;
            Renderer::HitPayload payload = TraceRay(ray);
            if (payload.HitDistance < 0.0f)
            {
                if (!m_ActiveScene->IsNight())
                    light += m_ActiveScene->GetSkyColor() * contribution;
                break;
            }

            // 从ECS中获取组件
            auto entity = payload.HitEntity;
            auto& material = m_Registry->get<MaterialComponent>(entity);

            contribution *= material.Albedo;
            light += material.GetEmission();

            ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;

            if (material.Metallic > 0.0f)
            {
                glm::vec3 reflected = glm::reflect(ray.Direction, payload.WorldNormal);
                ray.Direction = glm::normalize(reflected + material.Roughness * Utils::InUnitSphere(seed));
            }
            else
            {
                ray.Direction = glm::normalize(payload.WorldNormal + Utils::InUnitSphere(seed));
            }
        }

        return glm::vec4(light, 1.0f);
    }

    // 修改TraceRay方法
    Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
    {
		// (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
		// where
		// a = ray origin
		// b = ray direction
		// r = radius
		// t = hit distance
		
        Renderer::HitPayload closestPayload;
        closestPayload.HitDistance = -1.0f;
        float hitDistance = std::numeric_limits<float>::max();

        // 遍历所有具有SphereComponent和TransformComponent的实体
        auto view = m_Registry->view<TransformComponent, SphereComponent>();
        for (auto entity : view)
        {
            auto& transform = view.get<TransformComponent>(entity);
            auto& sphere = view.get<SphereComponent>(entity);

            glm::vec3 origin = ray.Origin - transform.Position;

            float a = glm::dot(ray.Direction, ray.Direction);
            float b = 2.0f * glm::dot(origin, ray.Direction);
            float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

            float discriminant = b * b - 4.0f * a * c;
            if (discriminant < 0.0f)
                continue;
			// Quadratic formula:
			// (-b +- sqrt(discriminant)) / 2a
			// float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a); // Second hit distance (currently unused)
			float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
            if (closestT > 0.0f && closestT < hitDistance)
            {
                hitDistance = closestT;
                closestPayload.HitDistance = hitDistance;
                closestPayload.HitEntity = entity;
            }
        }

        if (closestPayload.HitDistance < 0.0f)
            return Miss(ray);

        return ClosestHit(ray, closestPayload.HitDistance, closestPayload.HitEntity);
    }

    Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, entt::entity entity)
    {
        Renderer::HitPayload payload;
        payload.HitDistance = hitDistance;
        payload.HitEntity = entity;

        auto& transform = m_Registry->get<TransformComponent>(entity);
        auto& sphere = m_Registry->get<SphereComponent>(entity);

        glm::vec3 origin = ray.Origin - transform.Position;
        payload.WorldPosition = origin + ray.Direction * hitDistance;
        payload.WorldNormal = glm::normalize(payload.WorldPosition);

        payload.WorldPosition += transform.Position;

        return payload;
    }

    Renderer::HitPayload Renderer::Miss(const Ray& ray)
    {
        Renderer::HitPayload payload;
        payload.HitDistance = -1.0f;
        return payload;
    }
} // namespace RMC