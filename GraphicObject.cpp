#include "GraphicObject.h"

GraphicObject::GraphicObject(const createObjectInfo objectInfo, Device* deviceObj, Vertex* vertexObj, CommandBuffer* commandBufferObj, vulkan_render* renderer)
{
	
	this->rendererObj = renderer;
	this->OBJ_FILE_NAME = objectInfo.FILE_NAME_OBJ;
	this->TEXTURE_FILE_NAME = objectInfo.FILE_NAME_TEXTURE;


	if (incrementCount() == 1)
	{
		this->vertexObj = vertexObj;
		this->deviceObj = deviceObj;
		this->CommandBufferObj = commandBufferObj;		

		if (GraphicObject::OBJ_FILE_NAME.empty())
		{
			Log::error("Failed to load 3D OBJECT : no file");
			return;
		}
		this->vertexObj->loadModel(GraphicObject::OBJ_FILE_NAME, indices, vertices);
		if (GraphicObject::TEXTURE_FILE_NAME.size() == 0)
		{
			Log::message("the object has no texture");
		}
		for (size_t i = 0; i < GraphicObject::TEXTURE_FILE_NAME.size(); i++)
		{
			this->vertexObj->createTextureImage(GraphicObject::TEXTURE_FILE_NAME[i], GraphicObject::imageBuffers[i]);
		}
	}
	
	
	

	
	this->vertexObj->createIndexBuffer(indexBufferStruct, indices);
	this->vertexObj->createVertexBuffer(vertexBufferStruct,vertices);
	this->vertexObj->createDescriptorSets(GraphicObject::descriptorSets, GraphicObject::imageBuffers);
	CommandBufferObj->commandBufferLoad(rendererObj->getRenderPass(),
		rendererObj->getGraphicsPipeline(),
		rendererObj->getPipelineLayout(),
		&vertexBufferStruct.buffer,
		&indexBufferStruct.buffer,
		indices, GraphicObject::descriptorSets);
}

uint32_t GraphicObject::incrementCount()
{
	static uint32_t count = 0;
	return count++;
}




void GraphicObject::update()
{
	CommandBufferObj->commandBufferLoad(rendererObj->getRenderPass(),
		rendererObj->getGraphicsPipeline(),
		rendererObj->getPipelineLayout(),
		&vertexBufferStruct.buffer,
		&indexBufferStruct.buffer,
		indices, GraphicObject::descriptorSets);
}

void GraphicObject::cleanup()
{
	
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