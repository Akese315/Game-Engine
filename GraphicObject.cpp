#include "GraphicObject.h"

GraphicObject::GraphicObject(const createObjectInfo objectInfo, Vertex* vertexObj, CommandBuffer *CommandBufferObj, Device *deviceObj, vulkan_render * renderer)
{
	this->vertexObj = vertexObj;
	this->deviceObj = deviceObj;
	this->CommandBufferObj = CommandBufferObj;
	this->rendererObj = renderer;

	this->OBJ_FILE_NAME = objectInfo.FILE_NAME_OBJ;
	this->TEXTURE_FILE_NAME = objectInfo.FILE_NAME_TEXTURE;
	this->position.x = objectInfo.position[0];
	this->position.x = objectInfo.position[1];
	this->position.x = objectInfo.position[2];

	if (this->OBJ_FILE_NAME.empty())
	{
		Log::error("Failed to load 3D OBJECT : no file");
		return;
	}
	this->vertexObj->loadModel(OBJ_FILE_NAME, indices, vertices);
	if (this->TEXTURE_FILE_NAME.size()==0)
	{
		Log::message("the object has no texture");
	}
	for (size_t i = 0; i < this->TEXTURE_FILE_NAME.size(); i++)
	{
		this->vertexObj->createTextureImage(TEXTURE_FILE_NAME[i], imageBuffers[i]);
	}
	this->vertexObj->createIndexBuffer(indexBufferStruct, indices);
	this->vertexObj->createVertexBuffer(vertexBufferStruct,vertices);
	this->vertexObj->createDescriptorSets(descriptorSets, imageBuffers);
	CommandBufferObj->commandBufferLoad(rendererObj->getRenderPass(),
		rendererObj->getGraphicsPipeline(),
		rendererObj->getPipelineLayout(),
		&vertexBufferStruct.buffer,
		&indexBufferStruct.buffer,
		indices, descriptorSets);
}

GraphicObject::~GraphicObject()
{
	for (size_t i = 0; i < imageBuffers.size(); i++)
	{
		vkDestroyImage(deviceObj->getDevice(), imageBuffers[i].image, nullptr);
		vkFreeMemory(deviceObj->getDevice(), imageBuffers[i].memory, nullptr);
	}
	

	vkDestroyBuffer(deviceObj->getDevice(), indexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), indexBufferStruct.memory, nullptr);

	vkDestroyBuffer(deviceObj->getDevice(), vertexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), vertexBufferStruct.memory, nullptr);
}