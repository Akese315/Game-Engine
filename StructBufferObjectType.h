#pragma once
#include <vulkan/include/vulkan.h>
typedef struct __StructBufferObject
{
	VkBuffer buffer;
	VkBuffer staggingBuf;
	VkDeviceMemory staggingMem;
	VkDeviceMemory Memory;
	VkDeviceSize size;

}StructBufferObject;

typedef struct vertexStruc
{
	glm::vec2 pos;
	glm::vec3 color;
};

typedef struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};