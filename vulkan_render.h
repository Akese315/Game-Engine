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
	
	vector<VkFramebuffer>				_swapChainFramebuffers;
	VkRenderPass						_renderpass;
	Device*								deviceObj = NULL;
	VkExtent2D*							extent;
	SwapChain*							swapchain = NULL;
	VkDescriptorSetLayout descriptorLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;
	//materials 
	MaterialStruct normal;

	//TODO ->
	MaterialStruct metal;
	MaterialStruct glass;
	MaterialStruct water;
	//materials 


	vector<char>						readFile(const std::string& filename);
	VkShaderModule						createShaderModule(const std::vector<char>& code);	
	void								createMaterials(MaterialStruct& material);
	vector<char>						getShader(string vertexPath);
	void								init();
	VkRenderPass						createRenderPass();
	VkVertexInputBindingDescription			getBindingDescription();
	array<VkVertexInputAttributeDescription, 3>		getAttributeDescriptions();
	void								initShader();
	VkPipelineVertexInputStateCreateInfo createVertexInputState();
	VkPipelineInputAssemblyStateCreateInfo createInputAssembly();
	VkPipelineViewportStateCreateInfo createViewPortState();
	VkPipelineRasterizationStateCreateInfo createRasterizer();
	VkPipelineMultisampleStateCreateInfo createMultisampler();
	VkPipelineColorBlendStateCreateInfo createColorBlender();
	VkPipelineDepthStencilStateCreateInfo createDepthStencil();
	//VkPipelineDynamicStateCreateInfo createDynamicState();
	VkPipelineLayout createPipelineLayout(VkDescriptorSetLayout& descriptorSetLayout);
	
public:

										vulkan_render(Device* device, VkExtent2D* extent, SwapChain * swapchain);
										~vulkan_render();
	MaterialStruct						getNormalMaterial();
	VkPipeline							getPipeline();
	VkPipelineLayout*					getPipelineLayout();
	VkRenderPass*						getRenderPass();
	VkDescriptorSetLayout				createDescriptorSetLayout();
	 VkDescriptorSetLayout*				getDescriptorSetLayout();
	 void								recreatePipeline();
	 void								cleanUp();
};

