#include "Vertex.h"

Vertex::Vertex(Device* deviceObj, SwapChain* swapchainObj, CommandBuffer* commandBufferObj, vulkan_render* renderer) {
	this->deviceObj = deviceObj;
	this->swapchainObj = swapchainObj;
	this->CommandBufferObj = commandBufferObj;
	this->rendererObj = renderer;

	//récupère le descriptorSetLayout
	descriptorSetLayout = rendererObj->getDescriptorSetLayout();

	//initialition : Les buffers de verticies et d'index sont créer mais attendent que le commandBuffer les demande.
	createVertexBuffer();
	createIndexBuffer();	
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	CommandBufferObj->commandBufferLoad(rendererObj->getRenderPass(), 
		rendererObj->getGraphicsPipeline(), 
		rendererObj->getPipelineLayout(),
		&vertexBufferStruct.buffer,
		&indexBufferStruct.buffer, 
		indices, descriptorSets);
}




void Vertex::createBuffers(VkDeviceSize size, VkBufferUsageFlags usage,VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	//cela crée un Buffer
	VkBufferCreateInfo	bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//structure
	{
		VkResult result = vkCreateBuffer(deviceObj->getDevice(), &bufferInfo, nullptr, &buffer);
		if (result != VK_SUCCESS)
		{
			Log::error("failed to create vertexBuffer.", result);
			return;
		}
	}
	

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(deviceObj->getDevice(), buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = deviceObj->findMemoryType(memRequirements.memoryTypeBits, properties);
	{
		VkResult result = vkAllocateMemory(deviceObj->getDevice(), &allocInfo, nullptr, &bufferMemory);
		if (result != VK_SUCCESS)
		{

			Log::error("Failed to allocateMemory of verticies.", result);
			return;
		}
	}	
	{
		Log::success("Verticies memory allocated.");
		VkResult result = vkBindBufferMemory(deviceObj->getDevice(), buffer, bufferMemory, 0);
		if (result != VK_SUCCESS)
		{
			Log::error("Failed to bind memory with vertexBuffer.", result);
			return;
		}
	}
	

	
}

void Vertex::createVertexBuffer()
{	
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	StructBufferObject stagingBuffer{};


	createBuffers(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
					VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer.buffer, stagingBuffer.memory);
	void* data;
	vkMapMemory(deviceObj->getDevice(), stagingBuffer.memory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(deviceObj->getDevice(), stagingBuffer.memory);

	createBuffers(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferStruct.buffer,
		vertexBufferStruct.memory);

	CommandBufferObj->copyBuffer(&stagingBuffer, &vertexBufferStruct, bufferSize);

}


void Vertex::createIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
	StructBufferObject stagingBuffer{};

	createBuffers(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer.buffer,
		stagingBuffer.memory);

	void* data;
	vkMapMemory(deviceObj->getDevice(), stagingBuffer.memory, 0, bufferSize, 0,	&data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(deviceObj->getDevice(), stagingBuffer.memory);
	
		createBuffers(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferStruct.buffer,
			indexBufferStruct.memory);
		CommandBufferObj->copyBuffer(&stagingBuffer,&indexBufferStruct, bufferSize);
}


void Vertex::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	
		uniformBuffers.resize(swapchainObj->getSwapchainImage().size());
		uniformBuffersMemory.resize(swapchainObj->getSwapchainImage().size());
	
		for (size_t i = 0; i < swapchainObj->getSwapchainImage().size(); i++) {
			createBuffers(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i],
			uniformBuffersMemory[i]);
		
	}

}

void Vertex::updateUniformBuffer(uint32_t currentImage)
{
	// à chaque fois que la fonction draw est exécutée, elle appelle cette fonction qui en fonction 
		// du temps qui s'est écoulé depuis le début tourne tant le model sur un axe et une position donnée

		static auto startTime =	chrono::high_resolution_clock::now();		
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, chrono::seconds::period>(currentTime - startTime).count();

		uniformBuffer.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(144.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//le premier paramètre indique le model, le deuxième permet de faire une rotation de 90° en fonction du temps
		// et le dernier permet d'indiquer l'axe de rotation
		uniformBuffer.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f,
			0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// le premier paramètre : la position de l'oeil(caméra)
		//le deuxième : la position de la cible (ce que l'on regarde) inclinaison
		//le dernier est l'axe de la hauteur
		uniformBuffer.proj = glm::perspective(glm::radians(45.0f),
			swapchainObj->getCurrentWindowSize().width / (float)swapchainObj->getCurrentWindowSize().height, 0.1f,
			10.0f);
		//donne la perspective ici de 45° et les deux autre paramètre sont le ration de la taille de l'écran (ici)
		uniformBuffer.proj[1][1] *= -1;

		//la bibliothèque glm a été crée pour OPENGL qui inverse le positif et le négatif donc il faut tout inverser 
		void* data;
		vkMapMemory(deviceObj->getDevice(), uniformBuffersMemory[currentImage], 0, sizeof(uniformBuffer), 0, &data);
		memcpy(data, &uniformBuffer, sizeof(uniformBuffer));
		vkUnmapMemory(deviceObj->getDevice(), uniformBuffersMemory[currentImage]);
}
void Vertex::createDescriptorPool()
{	
	//Récupération de la taille du descripteur pool
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount =	static_cast<uint32_t>(swapchainObj->getSwapchainImage().size());

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(swapchainObj->getSwapchainImage().size());
	VkResult result = vkCreateDescriptorPool(this->deviceObj->getDevice(), &poolInfo, nullptr, &descriptorPool);
	if (result != VK_SUCCESS) {
		Log::error("Erreur lors de la création du descriptorPool", result);
	}
}

void Vertex::createDescriptorSets()
{
	
	std::vector<VkDescriptorSetLayout> layouts(swapchainObj->getSwapchainImage().size(), *descriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapchainObj->getSwapchainImage().size());
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(swapchainObj->getSwapchainImage().size());
	VkResult result = vkAllocateDescriptorSets(deviceObj->getDevice(), &allocInfo, descriptorSets.data());
	if (result != VK_SUCCESS) {
		Log::error("Erreur lors de l'allocation du descriptor sets", result);
	}
	for (size_t i = 0; i < swapchainObj->getSwapchainImage().size(); i++) {
		
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optionel
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(deviceObj->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}
	

}

void Vertex::recreateVertexObj()
{
	descriptorSetLayout = rendererObj->getDescriptorSetLayout();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	CommandBufferObj->commandBufferLoad(rendererObj->getRenderPass(),
		rendererObj->getGraphicsPipeline(),
		rendererObj->getPipelineLayout(),
		&vertexBufferStruct.buffer,
		&indexBufferStruct.buffer,
		indices, descriptorSets);
}

void Vertex::cleanUp()
{
	for (size_t i = 0; i < swapchainObj->getSwapchainImage().size(); i++) {
		vkDestroyBuffer(deviceObj->getDevice(), uniformBuffers[i], nullptr);
		vkFreeMemory(deviceObj->getDevice(), uniformBuffersMemory[i], nullptr);

	}

	vkDestroyDescriptorPool(deviceObj->getDevice(), descriptorPool, nullptr);	


	vkDestroyBuffer(deviceObj->getDevice(), indexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), indexBufferStruct.memory, nullptr);

	vkDestroyBuffer(deviceObj->getDevice(), vertexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), vertexBufferStruct.memory, nullptr);
}

Vertex::~Vertex()
{	
	cleanUp();
	vkDestroyDescriptorSetLayout(deviceObj->getDevice(), *descriptorSetLayout, nullptr);
}