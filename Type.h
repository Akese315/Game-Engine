#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <vulkan/include/vulkan.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

typedef struct StructBufferObject
{
	VkBuffer buffer;
	VkDeviceMemory memory;	
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;
	void* data;

}StructBufferObject;

typedef struct StructImageObject
{
	VkImage image;
	VkDeviceMemory memory;
	VkImageView textureImageView;

}StructImageObject;

typedef struct position
{
	float x = 0;
	float y = 0;
	float z = 0;
}position;

typedef struct vertexStruc
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	bool operator==(const vertexStruc& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;

	}

}vertexStruc;

typedef struct createObjectInfo
{
	std::string FILE_NAME_OBJ;
	std::vector<std::string> FILE_NAME_TEXTURE;
	std::vector<uint32_t> indices;
	std::vector<vertexStruc> vertices;
	position pos;
}createObjectInfo;



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

typedef struct keyMapping
{
	bool movefront = false;
	bool moveback = false;
	bool moveleft = false;
	bool moveright = false;
	bool ismove = false;
}keyMapping;

typedef struct MaterialStruct
{
	VkPipeline _graphicPipepline;
	VkPipelineLayout _layoutPipeline;
	std::string _vertexShaderPath;
	std::string _fragmentShaderPath;
	VkDescriptorSetLayout _descriptorLayout;
}MaterialStruct;

typedef struct CommandInfo
{
	VkPipeline graphicPipeline;
	VkPipelineLayout layoutPipeline;
	VkBuffer* vertexBuffer;
	VkBuffer* indexBuffer;
	std::vector<VkDescriptorSet>* descriptorSets;
	uint32_t indiceCount;
};