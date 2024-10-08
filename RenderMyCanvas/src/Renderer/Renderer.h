#pragma once

#include "Walnut/Image.h"

#include <memory>
#include <glm/glm.hpp>
#include <entt.hpp>
#include "Camera/Camera.h"

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;
	void OnResize(uint32_t width, uint32_t height);
	virtual void Render(const Camera& camera);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

protected:
	virtual glm::vec4 PerPixel(glm::vec2 coord);
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};