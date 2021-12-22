#include "CommandBuffer.h";


CommandBuffer::CommandBuffer(Device* DeviceObj, SwapChain* swapchainObj, vulkan_render* render, VkExtent2D* extent, Vertex* vertexObj)
{
	this->DeviceObj = DeviceObj;
	this->swapchainObj = swapchainObj;
	this->renderer = render;
	this->extent = extent;	
	this->vertexObj = vertexObj;
	
	init();
}

CommandBuffer::~CommandBuffer()
{	
	cleanUp();	
	vkDestroyCommandPool(DeviceObj->getDevice(), _commandPool, nullptr);
}


void CommandBuffer::init()
{
	createFrameBuffer();//#1
	createCommandPoolForTemp();//il faut créer le commandPool temporaire avant car il doit envoyer les données (vertices et index)
	sendIndexBuffer();//Elle ne sera utiliser qu'une seule fois sauf si la fenêtre change.
	sendVerticesBuffer();//Elle ne sera utiliser qu'une seule fois sauf si la fenêtre change.
	createCommandPool();//#2
	createCommandBuffer();//#3
}

void CommandBuffer::recreateCommandObj()
{
	init();
}

void CommandBuffer::cleanUp()
{
	for (VkFramebuffer framebuffer : _swapChainFramebuffers) {		   
		vkDestroyFramebuffer(DeviceObj->getDevice(), framebuffer, nullptr);	 
	}
	vkFreeCommandBuffers(DeviceObj->getDevice(),
		_commandPool, static_cast<uint32_t>(_swapChainFramebuffers.size()),
		_commandBuffer.data());											  
			 
																		
	
}

void CommandBuffer::createCommandPool()
{

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType						= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex			= DeviceObj->getGraphics_family_index();
	poolInfo.flags						= 0;


	VkResult error = vkCreateCommandPool(DeviceObj->getDevice(), &poolInfo, nullptr, &_commandPool);
	if (error != VK_SUCCESS) {
		Log::error("Failed to create Command Pool", error);
	}
}

void CommandBuffer::createCommandBuffer()
{
	_commandBuffer.resize(_swapChainFramebuffers.size());


	VkCommandBufferAllocateInfo command_buffer_allocate_info{};
	command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.commandPool = _commandPool;
	command_buffer_allocate_info.commandBufferCount = (uint32_t)_commandBuffer.size();
	command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VkResult error = vkAllocateCommandBuffers(DeviceObj->getDevice(), &command_buffer_allocate_info, _commandBuffer.data());
	if (error != VK_SUCCESS)
	{
		Log::error("Failed to allocate command buffers", error);
	}

	commandBufferLoad();
}
void CommandBuffer::commandBufferLoad()
{
	for (size_t i = 0; i < _commandBuffer.size(); i++)
	{

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags				= 0;
		beginInfo.pInheritanceInfo	= nullptr;


		VkResult error = vkBeginCommandBuffer(_commandBuffer[i], &beginInfo);
		if (error != VK_SUCCESS)
		{
			Log::error("erreur au début de l'enregistrement d'un command buffer!", error);
		}


		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass			= renderer->getRenderPass();
		renderPassInfo.framebuffer			= _swapChainFramebuffers[i];
		VkClearValue clearColor				= { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount		= 1;
		renderPassInfo.pClearValues			= &clearColor;
		renderPassInfo.renderArea.extent	= *extent;

		vkCmdBeginRenderPass(_commandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		//use render pass info 
		vkCmdBindPipeline(_commandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->getPipeline());
		//use pipeline 

		VkBuffer vertexBuffers[] = { vertexObj->getVertexBuffer().buffer };
		VkBuffer indexBuffer[] = { vertexObj->getIndexBuffer().buffer };
		vector<uint16_t> indices = vertexObj->getIndices();
		VkDeviceSize offsets[] = { 0 };

		//get all vertices indices and index
		vkCmdBindVertexBuffers(_commandBuffer[i], 0, 1, vertexBuffers,	offsets);
		vkCmdBindIndexBuffer(_commandBuffer[i], *indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(_commandBuffer[i],static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
		vkCmdEndRenderPass(_commandBuffer[i]);


		error = vkEndCommandBuffer(_commandBuffer[i]);
		if (error != VK_SUCCESS) {
			Log::error("Failed to load the command buffer", error);
		}
	}

}

void CommandBuffer::createCommandPoolForTemp()
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = DeviceObj->getGraphics_family_index();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

	VkResult error = vkCreateCommandPool(DeviceObj->getDevice(), &poolInfo, nullptr, &_commandPoolTemp);
	if (error != VK_SUCCESS) {
		Log::error("Failed to create Command Pool", error);

	}
}

void CommandBuffer::copyBuffer(Vertex::bufferStruct& buffer)
{
	// copie un buffer visible pour le CPU vers un buffer visible par le GPU
	
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _commandPoolTemp;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(DeviceObj->getDevice(), &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
	if (result != VK_SUCCESS)
	{
		Log::error("failed to beginCommandBuffer", result);
		return;
	}
	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optionel
	copyRegion.dstOffset = 0; // Optionel
	copyRegion.size = buffer.size;
	vkCmdCopyBuffer(commandBuffer, buffer.staggingBuf, buffer.buffer, 1, &copyRegion);
	result = vkEndCommandBuffer(commandBuffer);
	if (result != VK_SUCCESS)
	{
		Log::error("failed to end CommandBuffer", result);
		return;
	}
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	
	result = vkQueueSubmit(DeviceObj->getQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	if (result != VK_SUCCESS)
	{
		Log::error("failed to submit the queue", result);
		return;
	}
	result = vkQueueWaitIdle(DeviceObj->getQueue());
	if (result != VK_SUCCESS)
	{
		Log::error("failed to waitIdle", result);
		return;
	}
	vkDestroyBuffer(DeviceObj->getDevice(), buffer.staggingBuf, nullptr);
	vkFreeMemory(DeviceObj->getDevice(), buffer.staggingMem, nullptr);
}


void CommandBuffer::sendIndexBuffer()
{
	//send a command to copy a buffer
	Vertex::bufferStruct indexBuffer = vertexObj->getIndexBuffer();	
	copyBuffer(indexBuffer);
}

void CommandBuffer::sendVerticesBuffer()
{
	//send a command to copy a buffer
	Vertex::bufferStruct VerticesBuffer = vertexObj->getVertexBuffer();
	copyBuffer(VerticesBuffer);
}

void CommandBuffer::createFrameBuffer()
{
	_swapChainFramebuffers.resize(swapchainObj->getSwapChainImageView().size());

	for (size_t i = 0; i < _swapChainFramebuffers.size(); i++) {
		VkImageView attachments[] = { (swapchainObj->getSwapChainImageView()).at(i) };


		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType				= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass			= renderer->getRenderPass();
		framebufferInfo.attachmentCount		= 1;
		framebufferInfo.pAttachments		= attachments;
		framebufferInfo.width				= extent->width;
		framebufferInfo.height				= extent->height;
		framebufferInfo.layers				= 1;


		VkResult err = vkCreateFramebuffer(DeviceObj->getDevice(), &framebufferInfo, nullptr, &_swapChainFramebuffers[i]);
		if (err != VK_SUCCESS) {
			Log::error(string{ "Failed to create frame buffer" }, err);

		}
	}
	Log::success("Frame buffer created !");
}

vector<VkCommandBuffer> CommandBuffer::getCommandBuffer()
{

	return _commandBuffer;

}