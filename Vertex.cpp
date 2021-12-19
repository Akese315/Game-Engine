#include "Vertex.h"

Vertex::Vertex(Device *deviceObj) {
	this->deviceObj = deviceObj;

	//initialition : Les buffers de verticies et d'index sont créer mais attendent que le commandBuffer les demande.
	createVertexBuffer();
	createIndexBuffer();
}


VkVertexInputBindingDescription Vertex::getBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(vertexStruc);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}
array<VkVertexInputAttributeDescription, 2>	Vertex::getAttributeDescriptions()
{
	array<VkVertexInputAttributeDescription, 2 > attributeDescriptions{};
	attributeDescriptions[0].binding	= 0;
	attributeDescriptions[0].location	= 0;
	attributeDescriptions[0].format		= VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].location	= offsetof(vertexStruc, pos);
	 
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(vertexStruc, color);


	return attributeDescriptions;
}

void Vertex::createBuffers(VkDeviceSize size, VkBufferUsageFlags usage,VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	//cela crée un Buffer
	VkBufferCreateInfo	bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices[0]) * vertices.size();
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//structure
	VkResult result = vkCreateBuffer(deviceObj->getDevice(), &bufferInfo, nullptr, &buffer);
	if (result != VK_SUCCESS)
	{
		Log::error("failed to create vertexBuffer.", result);
		return;
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(deviceObj->getDevice(), buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = deviceObj->findMemoryType(memRequirements.memoryTypeBits, properties);

	result = vkAllocateMemory(deviceObj->getDevice(), &allocInfo, nullptr, &bufferMemory);
	if (result != VK_SUCCESS)
	{

		Log::error("Failed to allocateMemory of verticies.", result);
		return;
	}
	Log::success("Verticies memory allocated.");
	result = vkBindBufferMemory(deviceObj->getDevice(), buffer, bufferMemory, 0);
	if (result != VK_SUCCESS)
	{
		Log::error("Failed to bind memory with vertexBuffer.", result);
		return;
	}
}

void Vertex::createVertexBuffer()
{	
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	vertexBufferStruct.size = bufferSize;

	createBuffers(vertexBufferStruct.size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
					VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBufferStruct.staggingBuf, vertexBufferStruct.staggingMem);
	void* data;
	vkMapMemory(deviceObj->getDevice(), vertexBufferStruct.staggingMem, 0, vertexBufferStruct.size, 0, &data);
	memcpy(data, vertices.data(), (size_t)vertexBufferStruct.size);
	vkUnmapMemory(deviceObj->getDevice(), vertexBufferStruct.staggingMem);

	createBuffers(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferStruct.buffer,
		vertexBufferStruct.Memory);
}

void Vertex::createIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
	indexBufferStruct.size = bufferSize;
	createBuffers(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexBufferStruct.staggingBuf,
		indexBufferStruct.staggingMem);

	void* data;
	vkMapMemory(deviceObj->getDevice(), indexBufferStruct.staggingMem, 0, indexBufferStruct.size, 0,
		&data);
	memcpy(data, indices.data(), (size_t)indexBufferStruct.size);
	vkUnmapMemory(deviceObj->getDevice(), indexBufferStruct.staggingMem);
	
		createBuffers(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferStruct.buffer,
			indexBufferStruct.Memory);
}
Vertex::bufferStruct& Vertex::getVertexBuffer()
{
	return vertexBufferStruct;
}
Vertex::bufferStruct& Vertex::getIndexBuffer()
{
	return indexBufferStruct;
}

vector<uint16_t> Vertex::getIndices()
{
	return indices;
}

void Vertex::recreateVertexObj()
{
	createVertexBuffer();
	createIndexBuffer();
}

void Vertex::cleanUp()
{
	vkDestroyBuffer(deviceObj->getDevice(), indexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), indexBufferStruct.Memory, nullptr);

	vkDestroyBuffer(deviceObj->getDevice(), vertexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), vertexBufferStruct.Memory, nullptr);
}

Vertex::~Vertex()
{
	cleanUp();
}