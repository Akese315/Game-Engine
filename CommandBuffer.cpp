#include "CommandBuffer.h"


CommandBuffer::CommandBuffer(Device* DeviceObj, SwapChain* swapchainObj, VkExtent2D* extent, vulkan_render *renderer)
{
	this->DeviceObj = DeviceObj;
	this->swapchainObj = swapchainObj;
	this->extent = extent;	
	this->renderer = renderer;
	
	createCommandPool();//#2
	init();
}

CommandBuffer::~CommandBuffer()
{	
	cleanUp();	
	vkDestroyCommandPool(DeviceObj->getDevice(), _commandPool, nullptr);
	vkDestroyCommandPool(DeviceObj->getDevice(), _commandPoolTemp, nullptr);

}


void CommandBuffer::init()
{
	createCommandPoolForTemp();//il faut créer le commandPool temporaire avant car il doit envoyer les données (vertices et index)	

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
	_commandBuffer.clear();
																		
	
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

}
void CommandBuffer::commandBufferLoad(vector<CommandInfo> objectList)
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

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass			= *renderer->getRenderPass();
		renderPassInfo.framebuffer			= _swapChainFramebuffers[i];
		renderPassInfo.clearValueCount		= static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues			= clearValues.data();
		renderPassInfo.renderArea.extent	= *extent;

		
		VkDeviceSize offsets[] = { 0 };

		vkCmdBeginRenderPass(_commandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		for (CommandInfo object3D : objectList)
		{
			
			vkCmdBindPipeline(_commandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, object3D.graphicPipeline);
			vkCmdBindVertexBuffers(_commandBuffer[i], 0, 1, object3D.vertexBuffer, offsets);
			vkCmdBindIndexBuffer(_commandBuffer[i], *object3D.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
			if (object3D.descriptorSets->size() > 0)
			{
				vkCmdBindDescriptorSets(_commandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS,	object3D.layoutPipeline,
					0, 1,
					&object3D.descriptorSets->at(i),
					0, nullptr);
				
			}
			vkCmdDrawIndexed(_commandBuffer[i], static_cast<uint32_t>(object3D.indiceCount), 1, 0, 0, 0);
		}
		
		vkCmdEndRenderPass(_commandBuffer[i]);

		error = vkEndCommandBuffer(_commandBuffer[i]);
		if (error != VK_SUCCESS) {
			Log::error("Failed to load the command buffer", error);
		}
		string number = std::to_string(i);
		Log::message<string>(string{ "Envoie du commandbuffer numero : " }+number);
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

void CommandBuffer::copyBuffer(StructBufferObject* bufferStruct, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optionel
	copyRegion.dstOffset = 0; // Optionel
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, bufferStruct->stagingBuffer, bufferStruct->buffer, 1, &copyRegion);

	endSingleTimeCommands(commandBuffer);

	//vkDestroyBuffer(DeviceObj->getDevice(), srcBuffer->buffer, nullptr);
	//vkFreeMemory(DeviceObj->getDevice(), srcBuffer->memory, nullptr); déplacement 
}


void CommandBuffer::createFrameBuffer(VkImageView depthImageView)
{
	_swapChainFramebuffers.resize(swapchainObj->getSwapChainImageView().size());

	for (size_t i = 0; i < _swapChainFramebuffers.size(); i++) {
		std::array<VkImageView, 2> attachments = {
			(swapchainObj->getSwapChainImageView()).at(i),
			depthImageView

		};


		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType				= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass			= *renderer->getRenderPass();
		framebufferInfo.attachmentCount		= static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments		= attachments.data();
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

VkCommandBuffer CommandBuffer::beginSingleTimeCommands()
{
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
	
	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	
	return commandBuffer;

}

void CommandBuffer::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
	VkResult error = vkEndCommandBuffer(commandBuffer);
	if (error != VK_SUCCESS)
	{
		Log::error("failed to end CommandBuffer", error);
	}

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;

	submitInfo.pCommandBuffers = &commandBuffer;
	
	error = vkQueueSubmit(DeviceObj->getQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	if (error != VK_SUCCESS)
	{
		Log::error("Failed to subit to the queue", error);
	}
	error = vkQueueWaitIdle(DeviceObj->getQueue());
	if (error != VK_SUCCESS)
	{
		Log::error("Failed to wait Idle", error);
	}
	
	vkFreeCommandBuffers(DeviceObj->getDevice(), _commandPoolTemp, 1, &commandBuffer);

}
vector<VkCommandBuffer> CommandBuffer::getCommandBuffer()
{

	return _commandBuffer;

}