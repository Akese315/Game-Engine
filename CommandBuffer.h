#pragma once
#include <vulkan/include/vulkan.h>
#include "Type.h"
#include "Device.h"
#include "SwapChain.h"
#include "vulkan_render.h"
#include "Log.h"
#include <vector>


class CommandBuffer
{
	

public:
										CommandBuffer(Device* DeviceObj, SwapChain* swapchainObj, VkExtent2D* extent, vulkan_render *renderer);
										~CommandBuffer();
	vector<VkCommandBuffer>				getCommandBuffer();
	void								recreateCommandObj();
	void								copyBuffer(StructBufferObject* srcBuffer, StructBufferObject* destBuffer, VkDeviceSize size);
	void								cleanUp();
	void								commandBufferLoad(VkRenderPass* renderpass, VkPipeline* graphicPipeline, VkPipelineLayout* layoutPipeline,
										VkBuffer* vertex, VkBuffer* index, vector<uint16_t> indices, vector<VkDescriptorSet> descriptorSetList);
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

	Device*								DeviceObj;
	SwapChain*							swapchainObj;
	VkExtent2D*							extent;
	vulkan_render*						renderer;
	StructImageObject					depthBuffer;
};

