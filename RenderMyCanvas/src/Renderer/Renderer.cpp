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
namespace RMC
{
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

	void Renderer::Render(const Scene& scene, const Camera& camera)
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
				glm::vec4 color = TraceRay(scene, ray);

				color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

				m_ImageData[y * m_FinalImage->GetWidth() + x] = Utils::ConvertToRGBA(color);
			}
		}
		m_FinalImage->SetData(m_ImageData);
	}

	glm::vec4 Renderer::TraceRay(const Scene& scene, const Ray& ray)
	{
		//glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);
		//glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
		//float radius = 0.5f;

		if (scene.Spheres.size() == 0)
			return glm::vec4(0, 0, 0, 1);

		const Sphere* closestSphere = nullptr;
		float hitDistance = std::numeric_limits<float>::max();

		for (const Sphere& sphere : scene.Spheres)
		{
			glm::vec3 origin = ray.Origin - sphere.Position;
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
			if (closestT < hitDistance)
			{
				hitDistance = closestT;
				closestSphere = &sphere;
			}
		}

		if (closestSphere == nullptr)
			return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		glm::vec3 origin = ray.Origin - closestSphere->Position;
		glm::vec3 hitPoint = origin + ray.Direction * hitDistance;
		glm::vec3 normal = glm::normalize(hitPoint);

		glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

		float diff = glm::max(glm::dot(normal, -lightDir), 0.0f);

		glm::vec3 sphereColor = closestSphere->Albedo;
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
} // namespace RMC