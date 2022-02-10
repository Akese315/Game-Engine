#pragma once
#include <vulkan/include/vulkan.h>
#include <vector>
#include "Device.h"
#include "SwapChain.h"
#include "vulkan_render.h"
#include "Type.h"
#include "Log.h"

//#include "object.h"


class CommandBuffer
{
	

public:
										CommandBuffer(Device* DeviceObj, SwapChain* swapchainObj, VkExtent2D* extent, vulkan_render *renderer);
										~CommandBuffer();
	vector<VkCommandBuffer>				getCommandBuffer();
	void								recreateCommandObj();
	void								copyBuffer(StructBufferObject* srcBuffer, StructBufferObject* destBuffer, VkDeviceSize size);
	void								cleanUp();
	void								commandBufferLoad(vector<CommandInfo> objectList);
	VkCommandBuffer						beginSingleTimeCommands();
	void								endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void								createFrameBuffer(VkImageView depthImageView);
	void								createCommandBuffer();
	
	

	

private:
	void								createCommandPool();
	
	
	
	void								createCommandPoolForTemp();
	void								init();
	
				
	vector<VkFramebuffer>				_swapChainFramebuffers;
	VkCommandPool						_commandPool;
	VkCommandPool						_commandPoolTemp;
	vector<VkCommandBuffer>				_commandBuffer;

	//pointeur
	Device*								DeviceObj;
	SwapChain*							swapchainObj;
	VkExtent2D*							extent;
	vulkan_render*						renderer;
};

