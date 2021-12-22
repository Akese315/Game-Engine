#pragma once
#include <glm/glm.hpp>
#include "Log.h"
#include <vector>
#include <array>
#include "Device.h"
#define	GLM_FORCE_RADIANS



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

	Vertex(Device* deviceObj);
	~Vertex();
	void cleanUp();
	void recreateVertexObj();
	void setColor(string color);
	bufferStruct& getVertexBuffer();
	bufferStruct& getIndexBuffer();
	vector<uint16_t> getIndices();
	static VkVertexInputBindingDescription getBindingDescription();
	static array<VkVertexInputAttributeDescription, 2>	getAttributeDescriptions();


private:

	uint32_t vertexCount;
	Device* deviceObj;


	struct vertexStruc
	{
		glm::vec2 pos;
		glm::vec3 color;
	};
	
	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		
	};
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
	
	
};


