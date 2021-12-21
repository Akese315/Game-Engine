#include "vulkan_render.h"

vulkan_render::vulkan_render(Device* device, VkExtent2D* extent, SwapChain * swapchain, Vertex* vertex)
{
	this->swapchain		= swapchain;
	this->extent		= extent;
	this->device		= device;
	this->vertexObj		= vertex;

	init();
	
}

void vulkan_render::initShader()
{
	getShaderCode();
	VkShaderModule vertShaderModule = createShaderModule(_vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(_fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";
	_shaderStages[0] = fragShaderStageInfo;
	_shaderStages[1] = vertShaderStageInfo;
}


void vulkan_render::recreatePipeline()
{
	
	init();
}


void vulkan_render::cleanUp()
{
	vkDestroyPipeline(device->getDevice(), _graphicsPipeline, nullptr);	
	vkDestroyPipelineLayout(device->getDevice(), _pipelineLayout, nullptr);
	vkDestroyRenderPass(device->getDevice(), _renderPass, nullptr);
}
 

void vulkan_render::init()
{
	
	initShader();

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();


	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType							=	VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount	= 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());;
	vertexInputInfo.pVertexBindingDescriptions		= &bindingDescription; 
	vertexInputInfo.pVertexAttributeDescriptions	= attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType						=	VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology					= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable	=	VK_FALSE;
	/*
	
	VK_PRIMITIVE_TOPOLOGY_POINT_LIST = 0,
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST = 1,
    VK_PRIMITIVE_TOPOLOGY_LINE_STRIP = 2,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST = 3,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP = 4,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN = 5,
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY = 6,
    VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY = 7,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY = 8,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY = 9,
	
	*/
	VkViewport viewport{};
	viewport.x			= 0.0f;
	viewport.y			= 0.0f;
	viewport.width =	(float)extent->width;
	viewport.height		= (float)extent->height;
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;

	

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = *extent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType			=	VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount =	1;
	viewportState.pViewports	=	&viewport;
	viewportState.scissorCount	=	1;
	viewportState.pScissors		=	&scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType					=	VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable			=	VK_FALSE;
	rasterizer.rasterizerDiscardEnable	=	VK_FALSE;
	rasterizer.polygonMode				=	VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth				=	1.0f;
	rasterizer.cullMode					=	VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace				=	VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable			=	VK_FALSE;
	rasterizer.depthBiasConstantFactor	=	0.0f; // Optionel
	rasterizer.depthBiasClamp			=	0.0f; // Optionel
	rasterizer.depthBiasSlopeFactor		=	0.0f; // Optionel

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType					=	VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable	=	VK_FALSE;
	multisampling.rasterizationSamples	=	VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading		=	1.0f; // Optionel
	multisampling.pSampleMask			=	nullptr; // Optionel
	multisampling.alphaToCoverageEnable =	VK_FALSE; // Optionel
	multisampling.alphaToOneEnable		=	VK_FALSE; // Optionel

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask			= VK_COLOR_COMPONENT_R_BIT |VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable			= VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor	= VK_BLEND_FACTOR_ONE; //	Optionel
	colorBlendAttachment.dstColorBlendFactor	= VK_BLEND_FACTOR_ZERO; //	Optionel
	colorBlendAttachment.colorBlendOp			= VK_BLEND_OP_ADD; // Optionnel
	colorBlendAttachment.srcAlphaBlendFactor	= VK_BLEND_FACTOR_ONE; //	Optionel
	colorBlendAttachment.dstAlphaBlendFactor	= VK_BLEND_FACTOR_ZERO; //	Optionel
	colorBlendAttachment.alphaBlendOp			= VK_BLEND_OP_ADD; // Optionnel

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType				=	VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable		=	VK_FALSE;
	colorBlending.logicOp			=	VK_LOGIC_OP_COPY; // Optionel
	colorBlending.attachmentCount	=	1;
	colorBlending.pAttachments		=	&colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optionel
	colorBlending.blendConstants[1] = 0.0f; // Optionel
	colorBlending.blendConstants[2] = 0.0f; // Optionel
	colorBlending.blendConstants[3] = 0.0f; // Optionel

	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
		};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType				=VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount	= 2;
	dynamicState.pDynamicStates		= dynamicStates;

	createRenderPass();
	createPipelineLayout();
	

	VkGraphicsPipelineCreateInfo  pipelineInfo{};
	pipelineInfo.sType					= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount				= 2;
	pipelineInfo.pStages				= _shaderStages;
	pipelineInfo.pVertexInputState		= &vertexInputInfo;
	pipelineInfo.pInputAssemblyState	= &inputAssembly;
	pipelineInfo.pViewportState			= &viewportState;
	pipelineInfo.pRasterizationState	= &rasterizer;
	pipelineInfo.pMultisampleState		= &multisampling;
	pipelineInfo.pDepthStencilState		= nullptr; // Optionel
	pipelineInfo.pColorBlendState		= &colorBlending;
	pipelineInfo.pDynamicState			= nullptr;
	pipelineInfo.layout					= _pipelineLayout;
	pipelineInfo.renderPass				= _renderPass;
	pipelineInfo.subpass				= 0;
	pipelineInfo.basePipelineHandle		= NULL; // Optionel
	pipelineInfo.basePipelineIndex		= -1; // Optionel

	
	
	VkResult error = vkCreateGraphicsPipelines(device->getDevice(),nullptr,1,&pipelineInfo, nullptr, &_graphicsPipeline);
	
	if (error != VK_SUCCESS) {
		Log::error("Failed to create graphical pipeline!", error);
		exit(-1);
	}
	vkDestroyShaderModule(device->getDevice(), _shaderStages[0].module, nullptr);
	vkDestroyShaderModule(device->getDevice(), _shaderStages[1].module, nullptr);
	
	

}


VkShaderModule vulkan_render::createShaderModule(const std::vector<char>& code)
{

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	VkShaderModule shaderModule;
	VkResult error = vkCreateShaderModule(device->getDevice(), &createInfo, nullptr, &shaderModule);
	if (error != VK_SUCCESS)
	{
		Log::error("failed to create shader", error);

	}
	Log::success("Shader created");
	return shaderModule;

}


void vulkan_render::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount			= 1; // Optionel
	pipelineLayoutInfo.pSetLayouts				= vertexObj->getDescriptorSetLayout(); // Optionel
	pipelineLayoutInfo.pushConstantRangeCount	= 0; // Optionel
	pipelineLayoutInfo.pPushConstantRanges		= nullptr; // Optionel	

	VkResult err = vkCreatePipelineLayout(device->getDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout);
	if (err != VK_SUCCESS) {
		Log::error("Failed to create pipelineLayout", err);
		exit(-1);
	}
	Log::success("PipelineLayout created.");
}


void vulkan_render::createRenderPass()
{
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapchain->getFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;
	VkResult error = vkCreateRenderPass(device->getDevice(), &renderPassInfo, nullptr, &_renderPass);
	if (error != VK_SUCCESS) {
		Log::error("Failed to create RenderPass", error);

	}
	Log::success("RenderPass created");
}


void vulkan_render::getShaderCode()
{

	_vertShaderCode = readFile("shaders/vert.spv");
	_fragShaderCode = readFile("shaders/frag.spv");

}


std::vector<char> vulkan_render::readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		Log::error(string{ "failed to read the shader file : " }+filename);
		exit(-1);
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}


VkRenderPass vulkan_render::getRenderPass()
{
	return _renderPass;
}


VkPipeline vulkan_render::getGraphicsPipeline()
{
	return _graphicsPipeline;
}
VkPipelineLayout vulkan_render::getPipelineLayout()
{
	return _pipelineLayout;
}

vulkan_render::~vulkan_render()
{	
	cleanUp();
}

