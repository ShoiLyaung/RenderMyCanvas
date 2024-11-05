#pragma once
#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>
#include <entt.hpp>

class PostProcessingBase{
public:
	virtual uint32_t* process(uint32_t* image_data, int width, int height) = 0;
};

