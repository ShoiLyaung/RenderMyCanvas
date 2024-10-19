#pragma once

#include "Walnut/Application.h"
#include "Walnut/Image.h"

#include <memory>
#include <glm/glm.hpp>
#include <entt.hpp>
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
		Renderer();
		~Renderer() = default;
		void OnResize(uint32_t width, uint32_t height);
		virtual void Render(const Scene& scene, const Camera& camera);
		std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
		ImVec2 Get_uv0() const { return m_uv0; }
		ImVec2 Get_uv1() const { return m_uv1; }
	protected:
		glm::vec4 TraceRay(const Scene& scene, const Ray& ray);
		// virtual glm::vec4 PerPixel(glm::vec2 coord);
		std::shared_ptr<Walnut::Image> m_FinalImage;
		uint32_t* m_ImageData = nullptr;
		ImVec2 m_uv0 = { 0, 1 }, m_uv1 = { 1, 0 };
		std::unique_ptr < PostProcessingPipeLine > m_PpPipeline;
	};
} // namespace RMC