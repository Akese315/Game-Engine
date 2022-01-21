#pragma once
#include "Log.h"
#include <vector>
#include <fstream>
#include "Device.h"
#include "SwapChain.h"
#include "Type.h"
#include <array>
#include <vulkan/include/vulkan.h>
class vulkan_render
{
private:
	vector<char>						_vertShaderCode;
	vector<char>						_fragShaderCode;
	VkPipelineShaderStageCreateInfo		_shaderStages[2];
	vector<VkFramebuffer>				_swapChainFramebuffers;
	VkRenderPass						_renderPass = NULL;
	VkPipelineLayout					_pipelineLayout = NULL;
	VkPipeline							_graphicsPipeline = NULL;
	VkDescriptorSetLayout				_descriptorSetLayout = NULL;

	Device*								deviceObj = NULL;
	VkExtent2D*							extent;
	SwapChain*							swapchain = NULL;


	vector<char>						readFile(const std::string& filename);
	VkShaderModule						createShaderModule(const std::vector<char>& code);	
	
	void								getShaderCode();
	void								init();
	void								createRenderPass();
	VkVertexInputBindingDescription			getBindingDescription();
	array<VkVertexInputAttributeDescription, 3>		getAttributeDescriptions();
	void								initShader();
	
	
public:

										vulkan_render(Device* device, VkExtent2D* extent, SwapChain * swapchain);
										~vulkan_render();
	void								createPipelineLayout();
	void								createDescriptorSetLayout();
	 VkRenderPass*						getRenderPass();
	 VkPipeline*							getGraphicsPipeline();
	 VkPipelineLayout*					getPipelineLayout();
	 VkDescriptorSetLayout*				getDescriptorSetLayout();
	 void								recreatePipeline();
	 void								cleanUp();
};

