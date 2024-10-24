#pragma once

#include "Walnut/Application.h"
#include "Walnut/Image.h"

#include <memory>
#include <glm/glm.hpp>
#include "Camera/Camera.h"
#include "Camera/Ray.h"
#include "Scene/Scene.h"
#include "PostProcessing/PostProcessingPipeLine.h"
#include "PostProcessing/DLSSProcess.h"

namespace RMC
{
	class Renderer
	{
	public:
		struct Settings
		{
			bool Accumulate = true;
		};
		Renderer();
		~Renderer() = default;
		void OnResize(uint32_t width, uint32_t height);
		//virtual void Render(const Scene& scene, const Camera& camera);
		virtual void Render(const Scene& scene, const Camera& camera);

		std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
		uint32_t GetImageScale() const { return m_ImageScale; }
		ImVec2 Get_uv0() const { return m_uv0; }
		ImVec2 Get_uv1() const { return m_uv1; }
		void ResetFrameIndex() { m_FrameIndex = 1; }
		Settings& GetSettings() { return m_Settings; }
		std::unique_ptr < PostProcessingPipeLine > m_PpPipeline;
	protected:
		int _width = 0, _height = 0;
		struct HitPayload
		{
			float HitDistance;
			entt::entity HitEntity;

			glm::vec3 WorldPosition;
			glm::vec3 WorldNormal;
		};

		glm::vec4 PerPixel(uint32_t x, uint32_t y);
		HitPayload TraceRay(const Ray& ray);
		HitPayload ClosestHit(const Ray& ray, float hitDistance, entt::entity entity);
		HitPayload Miss(const Ray& ray);

		std::shared_ptr<Walnut::Image> m_FinalImage;
		Settings m_Settings;
		std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

		const Scene* m_ActiveScene = nullptr;
		const Camera* m_ActiveCamera = nullptr;
		const entt::registry* m_Registry = nullptr;

		uint32_t m_ImageScale = 2;
		uint32_t* m_ImageData = nullptr;
		ImVec2 m_uv0 = { 0, 1 }, m_uv1 = { 1, 0 };
		glm::vec4* m_AccumulationData = nullptr;
		uint32_t m_FrameIndex = 1;
	};
} // namespace RMC