#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <vulkan/include/vulkan.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
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

typedef struct vertexStruc
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	bool operator==(const vertexStruc& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
		
	}
		
	

}vertexStruc;

template<> struct std::hash<vertexStruc> {
		size_t operator()(vertexStruc const& vertex) const {
			return ((std::hash<glm::vec3>()(vertex.pos) ^ (std::hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (std::hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};

typedef struct {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
}UniformBufferObject;