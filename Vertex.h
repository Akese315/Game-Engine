#pragma once
#include <glm/glm.hpp>
#include "Log.h"
#include <vector>
#include <array>
#include "SwapChain.h"
#include "Device.h"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#define GLM_FORCE_RADIANS





class Vertex
{

public:
	
	struct bufferStruct
	{
		VkBuffer buffer;
		VkBuffer staggingBuf;
		VkDeviceMemory staggingMem;
		VkDeviceMemory Memory;
		VkDeviceSize size;

	};

	//C'est une structure qui permet de rassembler le stagging buffer et le buffer
	// Elle est utilisée par les vertex et les index.

	Vertex(Device* deviceObj, SwapChain* swapchainObj);
	~Vertex();
	void cleanUp();
	void recreateVertexObj();
	void setColor(string color);
	bufferStruct& getVertexBuffer();
	bufferStruct& getIndexBuffer();
	VkDescriptorSetLayout* getDescriptorSetLayout();
	vector<uint16_t> getIndices();
	static VkVertexInputBindingDescription getBindingDescription();
	static array<VkVertexInputAttributeDescription, 2>	getAttributeDescriptions();
	void updateUniformBuffer(uint32_t index);
	vector<VkDescriptorSet> getDescriptorSetList();


private:

	uint32_t vertexCount = NULL;
	Device* deviceObj = NULL;
	SwapChain* swapchainObj = NULL;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	vector<VkBuffer> uniformBuffers;
	vector<VkDeviceMemory> uniformBuffersMemory;
	vector<VkDescriptorSet> descriptorSets;

	struct vertexStruc
	{
		glm::vec2 pos;
		glm::vec3 color;
	};
	
	struct UniformBufferObject {
		glm::mat4 model;				//64 byte
		glm::mat4 view;					//64 byte
		glm::mat4 proj;					//64 byte
		glm::mat4 memory_reserved;		//64 byte
	};

	// Un UBO a besoin d'avoir une taile de 256 bytes donc on en rajoute un juste pour compléter
	UniformBufferObject uniformBuffer;


	const vector<vertexStruc> vertices =
	{
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{ {0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{ {0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{ {-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	// vertex (coordonnées et couleurs)
	const vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };


	bufferStruct vertexBufferStruct;
	bufferStruct indexBufferStruct;

	

	//déclaration de la structure bufferStruc du vertex et des index.

	void createBuffers(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void createVertexBuffer();
	void createDescriptorSetLayout();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	
	
	
};


