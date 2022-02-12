#pragma once
#define STB_IMAGE_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL
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
#include <unordered_map>
#include <glm/gtx/hash.hpp>







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
	vector<VkDescriptorSet> getDescriptorSet();
	VkDescriptorSetLayout* getDescriptorSetLayout();
	vector<uint16_t> getIndices();
	static VkVertexInputBindingDescription getBindingDescription();
	static array<VkVertexInputAttributeDescription, 2>	getAttributeDescriptions();
	void loadModel(const string MODEL_PATH, vector<uint32_t>& indices, vector<vertexStruc>& vertices);
	void createTextureImage(const string TEXTURE_FILE_NAME, StructImageObject& imageBuffer);
	void createDescriptorSets(vector<VkDescriptorSet>& descriptorSets, vector<StructImageObject> imageBuffers, VkDescriptorPool* descriptorPool);
	void createDescriptorPool(VkDescriptorPool& descriptorPool);
	vector<StructBufferObject>* createUniformBuffers();
	void createBuffers(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

private:

	Device* deviceObj;
	SwapChain* swapchainObj;
	CommandBuffer* CommandBufferObj;
	vulkan_render* rendererObj;
	VkDescriptorSetLayout* descriptorSetLayout;

	VkSampler textureSampler;
	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;
	VkImageView depthImageView;

	vector<StructBufferObject> uniformBufferStruct;
	StructImageObject depthBuffer;
	
	//déclaration de la structure bufferStruc du vertex et des index.

	
		
	
	
	void createTextureImageView(StructImageObject& imageStruct);
	void createDepthResources();
	void createTextureSampler();	
	void transitionImageLayout(VkImage image, VkFormat format,
		VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t
		width, uint32_t height);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, StructImageObject& imageStruct);
	

};


