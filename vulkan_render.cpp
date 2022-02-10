#include "vulkan_render.h"

vulkan_render::vulkan_render(Device* deviceObj, VkExtent2D* extent, SwapChain * swapchain)
{
	this->swapchain		= swapchain;
	this->extent		= extent;
	this->deviceObj		= deviceObj;

	_renderpass = createRenderPass();
	createMaterials(normal);
	
}

VkVertexInputBindingDescription vulkan_render::getBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(vertexStruc);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}
array<VkVertexInputAttributeDescription, 3>	vulkan_render::getAttributeDescriptions()
{
	array<VkVertexInputAttributeDescription, 3 > attributeDescriptions{};
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].location = offsetof(vertexStruc, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(vertexStruc, color);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(vertexStruc, texCoord);

	return attributeDescriptions;
}


void vulkan_render::recreatePipeline()
{
	_renderpass = createRenderPass();
	createMaterials(normal);
}


void vulkan_render::cleanUp()
{
	//destruction du material normal (dépend du nombre de material)
	vkDestroyPipeline(deviceObj->getDevice(), normal._graphicPipepline, nullptr);	
	vkDestroyPipelineLayout(deviceObj->getDevice(), normal._layoutPipeline, nullptr);


	//destruction du render pass (il ne peut y avoir qu'in seul render pass
	//www.reddit.com/r/vulkan/comments/e7n5b6/drawing_multiple_objects/


	vkDestroyRenderPass(deviceObj->getDevice(), _renderpass, nullptr);
}
 

void vulkan_render::createMaterials(MaterialStruct& material)
{
	vector<char> vertexShader;
	vector<char> fragmentShader;
	material._vertexShaderPath = "shaders/vert.spv";
	material._fragmentShaderPath = "shaders/frag.spv";
	vertexShader = getShader(material._vertexShaderPath);
	fragmentShader = getShader(material._fragmentShaderPath);

	//charge les shader dans un vector

	VkShaderModule vertShaderModule = createShaderModule(vertexShader);
	VkShaderModule fragShaderModule = createShaderModule(fragmentShader);

	//compile le shader et le transforme en shaderModule

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

	

	//formate les shaderModule et les rassemble dans un shaderstages

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	//VkPipelineVertexInputStateCreateInfo VertexInput = createVertexInputState();

	array<VkVertexInputAttributeDescription,3>  attributeDescriptions = getAttributeDescriptions();
	VkVertexInputBindingDescription bindingDescription = getBindingDescription();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	//VkPipelineInputAssemblyStateCreateInfo inputAssembly = createInputAssembly();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	
	//VkPipelineViewportStateCreateInfo viewportState = createViewPortState();

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent->width;
	viewport.height = (float)extent->height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = *extent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	//VkPipelineColorBlendStateCreateInfo colorBlender = createColorBlender();

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	/*colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; //	Optionel
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; //	Optionel
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optionnel
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; //	Optionel
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; //	Optionel
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optionnel*/

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optionel
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optionel
	colorBlending.blendConstants[1] = 0.0f; // Optionel
	colorBlending.blendConstants[2] = 0.0f; // Optionel
	colorBlending.blendConstants[3] = 0.0f; // Optionel


	//VkPipelineRasterizationStateCreateInfo rasterizer = createRasterizer();

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	/*rasterizer.depthBiasConstantFactor = 0.0f; // Optionel
	rasterizer.depthBiasClamp = 0.0f; // Optionel
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optionel*/


	//VkPipelineMultisampleStateCreateInfo multiSampler = createMultisampler();

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	/*
	multisampling.minSampleShading = 1.0f; // Optionel
	multisampling.pSampleMask = nullptr; // Optionel
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optionel
	multisampling.alphaToOneEnable = VK_FALSE; // Optionel*/

	//VkPipelineDepthStencilStateCreateInfo depthStencil = createDepthStencil();

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	//depthStencil.minDepthBounds = 0.0f; // Optionel
	//depthStencil.maxDepthBounds = 1.0f; // Optionel
	depthStencil.stencilTestEnable = VK_FALSE;
	//depthStencil.front{}; // Optionel
	//depthStencil.back{}; // Optionel


	//VkPipelineDynamicStateCreateInfo dynamicState = createDynamicState();

	descriptorLayout = createDescriptorSetLayout();
	pipelineLayout = createPipelineLayout(descriptorLayout);

	VkGraphicsPipelineCreateInfo  pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil; // Optionel
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; //plus tard PLUS JAMAIS PUT1 à cause de cette merde j'ai galéré pendant 2 heures
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = _renderpass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = NULL; // Optionel
	


	
	
	cout << shaderStages << endl;
	cout << &inputAssembly << endl;
	cout << &viewportState << endl;
	cout << &rasterizer << endl;
	cout << &multisampling << endl;
	cout << &depthStencil << endl;
	cout << &colorBlending << endl;
	cout << &pipelineInfo << endl;
	cout << pipelineLayout << endl;
	cout << _renderpass << endl;
	

	
	VkResult error = vkCreateGraphicsPipelines(deviceObj->getDevice(), nullptr, 1, &pipelineInfo, nullptr, &pipeline);
	if (error != VK_SUCCESS) {
		Log::error("Failed to create graphical pipeline!", error);
		exit(-1);
	}
	normal._graphicPipepline = pipeline;
	normal._descriptorLayout =descriptorLayout;
	normal._layoutPipeline = pipelineLayout;

	vkDestroyShaderModule(deviceObj->getDevice(), shaderStages[0].module, nullptr);
	vkDestroyShaderModule(deviceObj->getDevice(), shaderStages[1].module, nullptr);
}

//			Les fonctions en bas 
//
//					||
//					||
//					||
//					||
//					||
//					||
//					||
//					||
//					||
//					||
//					||
//					||
//				    \/
// 
// 


VkPipelineVertexInputStateCreateInfo vulkan_render::createVertexInputState()
{
	auto bindingDescription = getBindingDescription();
	auto attributeDescriptions = getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo vulkan_render::createInputAssembly()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	return inputAssembly;
}

VkPipelineViewportStateCreateInfo vulkan_render::createViewPortState()
{
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent->width;
	viewport.height = (float)extent->height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;



	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = *extent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	return viewportState;
}

VkPipelineColorBlendStateCreateInfo vulkan_render::createColorBlender()
{

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; //	Optionel
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; //	Optionel
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optionnel
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; //	Optionel
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; //	Optionel
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optionnel

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optionel
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optionel
	colorBlending.blendConstants[1] = 0.0f; // Optionel
	colorBlending.blendConstants[2] = 0.0f; // Optionel
	colorBlending.blendConstants[3] = 0.0f; // Optionel

	return colorBlending;
}

VkPipelineRasterizationStateCreateInfo vulkan_render::createRasterizer()
{
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optionel
	rasterizer.depthBiasClamp = 0.0f; // Optionel
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optionel

	return rasterizer;
}

VkPipelineMultisampleStateCreateInfo vulkan_render::createMultisampler()
{
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optionel
	multisampling.pSampleMask = nullptr; // Optionel
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optionel
	multisampling.alphaToOneEnable = VK_FALSE; // Optionel

	return multisampling;
}

VkPipelineDepthStencilStateCreateInfo vulkan_render::createDepthStencil()
{
	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optionel
	depthStencil.maxDepthBounds = 1.0f; // Optionel
	depthStencil.stencilTestEnable = VK_FALSE;
	//depthStencil.front{}; // Optionel
	//depthStencil.back{}; // Optionel

	return depthStencil;
}
/*
VkPipelineDynamicStateCreateInfo vulkan_render::createDynamicState()
{
	
	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	return dynamicState;
}
*/


VkShaderModule vulkan_render::createShaderModule(const std::vector<char>& code)
{

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	VkShaderModule shaderModule;
	VkResult error = vkCreateShaderModule(deviceObj->getDevice(), &createInfo, nullptr, &shaderModule);
	if (error != VK_SUCCESS)
	{
		Log::error("failed to compile! shader", error);
		return NULL;
	}
	Log::success("Shader created");
	return shaderModule;

}


VkPipelineLayout vulkan_render::createPipelineLayout(VkDescriptorSetLayout& descriptorSetLayout)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount			= 1; // Optionel
	pipelineLayoutInfo.pSetLayouts				= &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount	= 0; // Optionel
	pipelineLayoutInfo.pPushConstantRanges		= nullptr; // Optionel	

	VkPipelineLayout _pipelineLayout;

	VkResult err = vkCreatePipelineLayout(deviceObj->getDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout);
	if (err != VK_SUCCESS) {
		Log::error("Failed to create pipelineLayout", err);
		exit(-1);
	}
	Log::success("PipelineLayout created.");

	return _pipelineLayout;
}




VkRenderPass vulkan_render::createRenderPass()
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

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = deviceObj->findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout =	VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	

	std::array<VkAttachmentDescription, 2> attachments =
	{ colorAttachment, depthAttachment };

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());;
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;
	VkRenderPass _renderPass;
	VkResult error = vkCreateRenderPass(deviceObj->getDevice(), &renderPassInfo, nullptr, &_renderPass);
	if (error != VK_SUCCESS) {
		Log::error("Failed to create RenderPass", error);

	}
	Log::success("RenderPass created");
	return _renderPass;
}

VkPipeline vulkan_render::getPipeline()
{
	return normal._graphicPipepline;
}

VkPipelineLayout* vulkan_render::getPipelineLayout()
{
	return &pipelineLayout;
}
VkDescriptorSetLayout vulkan_render::createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	//TODO -> un gros descriptor pour tout les models où l'on récupère le bon grâce à son binding

	std::array<VkDescriptorSetLayoutBinding, 2> bindings =	{ uboLayoutBinding, samplerLayoutBinding };
	


	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	VkDescriptorSetLayout _descriptorSetLayout;
	VkResult result = vkCreateDescriptorSetLayout(deviceObj->getDevice(), &layoutInfo, nullptr, &_descriptorSetLayout);
	if (result != VK_SUCCESS)
	{
		Log::error("Failed to create Descriptor set Layout.", result);
	}
	return _descriptorSetLayout;
	
}

MaterialStruct vulkan_render::getNormalMaterial()
{
	return normal;
}

VkRenderPass* vulkan_render::getRenderPass()
{
	return &_renderpass;
}


vector<char> vulkan_render::getShader(string vertexPath)
{
	return readFile((const string)vertexPath);
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

VkDescriptorSetLayout* vulkan_render::getDescriptorSetLayout()
{
	return &normal._descriptorLayout;
}

vulkan_render::~vulkan_render()
{	
	cleanUp();
}

