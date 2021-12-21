#pragma once
#include <vulkan/include/vulkan.h>
#include "Log.h"
#include <glm/glm.hpp>

class camera
{
private:
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;

	void createDescriptorSetLayout();

public:

};

