#pragma once
#define STB_IMAGE_IMPLEMENTATION
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vulkan/include/vulkan.h>
#include "Log.h"
#include <vector>
#include <array>
#include "SwapChain.h"
#include "Type.h"
#include "CommandBuffer.h"
#include "vulkan_render.h"
#include "Device.h"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>




class Vertex
{

	

public:
	
	
	//C'est une structure qui permet de rassembler le stagging buffer et le buffer
	// Elle est utilisée par les vertex et les index.

	Vertex(Device* deviceObj, SwapChain* swapchainObj, CommandBuffer* commandBufferObj, vulkan_render * renderer);
	~Vertex();
	void cleanUp();
	void recreateVertexObj();
	void setColor(string color);
	void updateUniformBuffer(uint32_t currentImage);
	StructBufferObject* getVertexBuffer() {
		return &vertexBufferStruct;
	}
	StructBufferObject* getIndexBuffer() { return &indexBufferStruct; }
	vector<VkDescriptorSet> getDescriptorSet();
	VkDescriptorSetLayout* getDescriptorSetLayout();
	vector<uint16_t> getIndices();
	static VkVertexInputBindingDescription getBindingDescription();
	static array<VkVertexInputAttributeDescription, 2>	getAttributeDescriptions();


private:

	uint32_t vertexCount;
	Device* deviceObj;
	SwapChain* swapchainObj;
	CommandBuffer* CommandBufferObj;
	vulkan_render* rendererObj;
	VkDescriptorSetLayout* descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	vector<VkDescriptorSet> descriptorSets;
	vector<VkBuffer> uniformBuffers;
	vector<VkDeviceMemory> uniformBuffersMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;
	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;
	VkImageView depthImageView;

	//const std::string MODEL_PATH = "models/nissan.obj";
	//const std::string TEXTURE_PATH = "textures/nissan.png";
	

	const vector<vertexStruc> vertices =
	{
		{{-0.5f, -0.5f,	0.0f}, {0.0f, 0.5f, 1.0f},{0.0f,0.0f}},
		{ {0.5f, -0.5f, 0.0f}, {0.5f, 0.5f, 0.0f},{1.0f,0.0f}},
		{ {0.5f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.0f},{1.0f,1.0f}},
		{ {-0.5f, 0.5f,	0.0f}, {0.0f, 0.5f, 1.0f},{0.0f,1.0f}},
		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{ {0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{ {0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{ {-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};
	// vertex (coordonnées et couleurs)
	const vector<uint16_t> indices = 
	{ 0, 1, 2, 2, 3, 0, 
	  4, 5, 6, 6, 7, 4
	};


	StructBufferObject vertexBufferStruct;
	StructBufferObject indexBufferStruct;
	StructImageObject imageBuffer;
	UniformBufferObject uniformBuffer;
	StructImageObject depthBuffer;
	//déclaration de la structure bufferStruc du vertex et des index.

	void createBuffers(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void createVertexBuffer();
	void createUniformBuffers();
	void createIndexBuffer();
	void createDescriptorPool();
	void createDescriptorSets();
	void createTextureImage();
	void createTextureImageView();
	void createDepthResources();
	void createTextureSampler();
	void transitionImageLayout(VkImage image, VkFormat format,
		VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t
		width, uint32_t height);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, StructImageObject& imageStruct);
	
};


