#include "Renderer.h"

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
}

void Renderer::Render()
{
	//render every pixel

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f;
			//m_ImageData[y * m_FinalImage->GetWidth() + x] = PerPixel(coord);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
	glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	float radius = 0.5f;

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0.0f)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
	float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);

	glm::vec3 hitPoint = rayOrigin + rayDirection * closestT;

	glm::vec3 sphereColor(1.0f, 0.0f, 1.0f);
	sphereColor *= hitPoint;

	return glm::vec4(sphereColor, 1.0f);
}

//void Renderer::Draw(entt::registry& registry) {
//	auto view = registry.view<LineComponent>();
//	for (auto entity : view) {
//		const auto& line = view.get<LineComponent>(entity);
//		DrawPrimitive(line);
//	}
//
//	auto circleView = registry.view<CircleComponent>();
//	for (auto entity : circleView) {
//		const auto& circle = circleView.get<CircleComponent>(entity);
//		DrawPrimitive(circle);
//	}
//}

void Renderer::AddPrimitive(std::shared_ptr<Primitive> primitive)
{
	//TODO: Implement this
}