#include "Renderer.h"

namespace Utils {
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint32_t r = (uint32_t)(color.r * 255.0f);
		uint32_t g = (uint32_t)(color.g * 255.0f);
		uint32_t b = (uint32_t)(color.b * 255.0f);
		uint32_t a = (uint32_t)(color.a * 255.0f);

		return (a << 24) | (b << 16) | (g << 8) | r;
	}
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
}

void Renderer::Render(const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();
	//render every pixel

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			//glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			//coord = coord * 2.0f - 1.0f;

			//glm::vec4 color = PerPixel(coord);

			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
			glm::vec4 color = TraceRay(ray);

			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

			m_ImageData[y * m_FinalImage->GetWidth() + x] = Utils::ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const Ray& ray)
{
	//glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);
	//glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	float radius = 0.5f;

	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2.0f * glm::dot(ray.Origin, ray.Direction);
	float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0.0f)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
	float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);

	glm::vec3 hitPoint = ray.Origin + ray.Direction * closestT;
	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

	float diff = glm::max(glm::dot(normal, -lightDir), 0.0f);

	glm::vec3 sphereColor(1.0f, 0.0f, 1.0f);
	sphereColor *= diff;

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
