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
	VkImage image;
	VkDeviceMemory memory;

}StructImageObject;

typedef struct 
{
	glm::vec2 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
}vertexStruc;

typedef struct {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
}UniformBufferObject;