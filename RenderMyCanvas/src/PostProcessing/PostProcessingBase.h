#pragma once
#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>
#include <entt.hpp>

class PostProcessingBase{
public:
	virtual std::shared_ptr<Walnut::Image> process(std::shared_ptr<Walnut::Image> img) = 0;
};

