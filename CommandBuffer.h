#pragma once
#include <vulkan/include/vulkan.h>
#include "Device.h"
#include "Vertex.h"
#include "SwapChain.h"
#include "vulkan_render.h"
#include "Log.h"
#include <vector>


class CommandBuffer
{
	

public:
										CommandBuffer(Device* DeviceObj, SwapChain* swapchainObj, vulkan_render* render, VkExtent2D* extent, Vertex* vertexObj);
										~CommandBuffer();
	vector<VkCommandBuffer>				getCommandBuffer();
	void								recreateCommandObj();
	void								init();
	void								cleanUp();
	
	

	
	

	

private:
	void								createCommandPool();
	void								createFrameBuffer();
	void								createCommandBuffer();
	void								commandBufferLoad();
	void								createCommandPoolForTemp();
	void                                sendIndexBuffer();
	void							    sendVerticesBuffer();
	void								copyBuffer(Vertex::bufferStruct& buffer);
	
	
				
	vector<VkFramebuffer>				_swapChainFramebuffers;
	VkCommandPool						_commandPool;
	VkCommandPool						_commandPoolTemp;
	vector<VkCommandBuffer>				_commandBuffer;

	Device*								DeviceObj;
	SwapChain*							swapchainObj;
	vulkan_render*						renderer;
	VkExtent2D*							extent;
	Vertex*								vertexObj;
};

