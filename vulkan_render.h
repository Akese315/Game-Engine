#pragma once
#include "Log.h"
#include <vector>
#include <fstream>
#include "Device.h"
#include "SwapChain.h"
#include "vertex.h"
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


	Device*								device = NULL;
	VkExtent2D*							extent;
	SwapChain*							swapchain = NULL;
	


	vector<char>						readFile(const std::string& filename);
	VkShaderModule						createShaderModule(const std::vector<char>& code);	
	
	void								getShaderCode();
	void								init();
	void								createRenderPass();
	void								createPipelineLayout();
	void								initShader();
	
	
public:

										vulkan_render(Device* device, VkExtent2D* extent, SwapChain * swapchain);
										~vulkan_render();
	 VkRenderPass						getRenderPass();
	 VkPipeline							getPipeline();
	 void								recreatePipeline();
	 void								cleanUp();
};

