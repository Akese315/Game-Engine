#pragma once
#include <vulkan/include/vulkan.h>
#include <glm/glm.hpp>
typedef struct
{
	VkBuffer buffer;
	VkDeviceMemory memory;
	
	

}StructBufferObject;

typedef struct 
{
	glm::vec2 pos;
	glm::vec3 color;
}vertexStruc;

typedef struct {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
}UniformBufferObject;