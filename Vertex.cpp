#include "Vertex.h"
#include <GLFW/stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <GLFW/tiny_obj_loader.h>


Vertex::Vertex(Device* deviceObj, SwapChain* swapchainObj, CommandBuffer* commandBufferObj, vulkan_render* renderer) {
	this->deviceObj = deviceObj;
	this->swapchainObj = swapchainObj;
	this->CommandBufferObj = commandBufferObj;
	this->rendererObj = renderer;

	//r�cup�re le descriptorSetLayout
	descriptorSetLayout = rendererObj->getDescriptorSetLayout();

	//initialition : Les buffers de verticies et d'index sont cr�er mais attendent que le commandBuffer les demande.
	createDepthResources();	
	createTextureSampler();
	createUniformBuffers();

	CommandBufferObj->createFrameBuffer(depthImageView);
	CommandBufferObj->createCommandBuffer();
	//changement de place de CommandBufferObj
}




void Vertex::createBuffers(VkDeviceSize size, VkBufferUsageFlags usage,VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	//cela cr�e un Buffer
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

void Vertex::createVertexBuffer(StructBufferObject &vertexBufferStruct, const vector<vertexStruc> vertices)
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


void Vertex::createIndexBuffer(StructBufferObject& indexBufferStruct,const vector<uint32_t> indices)
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
	// � chaque fois que la fonction draw est ex�cut�e, elle appelle cette fonction qui en fonction 
		// du temps qui s'est �coul� depuis le d�but tourne tant le model sur un axe et une position donn�e

		static auto startTime =	chrono::high_resolution_clock::now();		
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, chrono::seconds::period>(currentTime - startTime).count();

		uniformBuffer.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//le premier param�tre indique le model, le deuxi�me permet de faire une rotation de 90� en fonction du temps
		// et le dernier permet d'indiquer l'axe de rotation
		uniformBuffer.view = glm::lookAt(
			glm::vec3(7.0f, 0.0f, 7.0f),// le premier param�tre : la position de l'oeil(cam�ra)
			glm::vec3(0.0f,0.0f, 0.0f),//le deuxi�me : la position de la cible (ce que l'on regarde) inclinaison
			glm::vec3(0.0f, 1.0f, 0.0f));//le dernier est l'axe de la hauteur
		
		
		
		uniformBuffer.proj = glm::perspective(glm::radians(60.0f),swapchainObj->getCurrentWindowSize().width / (float)swapchainObj->getCurrentWindowSize().height, 0.1f, 10.0f);
		//donne la perspective ici de 45� et les deux autre param�tre sont le ration de la taille de l'�cran (ici)
		uniformBuffer.proj[1][1] *= -1;

		//la biblioth�que glm a �t� cr�e pour OPENGL qui inverse le positif et le n�gatif donc il faut tout inverser 
		void* data;
		vkMapMemory(deviceObj->getDevice(), uniformBuffersMemory[currentImage], 0, sizeof(uniformBuffer), 0, &data);
		memcpy(data, &uniformBuffer, sizeof(uniformBuffer));
		vkUnmapMemory(deviceObj->getDevice(), uniformBuffersMemory[currentImage]);
}
void Vertex::createDescriptorPool(VkDescriptorPool& descriptorPool)
{	
	//R�cup�ration de la taille du descripteur pool
	std::array<VkDescriptorPoolSize, 2> poolSizes{};

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount =	static_cast<uint32_t>(swapchainObj->getSwapchainImage().size());

	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount =	static_cast<uint32_t>(swapchainObj->getSwapchainImage().size());

	//nombre de buffer de le descripteur
	// Je cr�er 2 descripteurs pour 2 models

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(swapchainObj->getSwapchainImage().size());

	VkResult result = vkCreateDescriptorPool(this->deviceObj->getDevice(), &poolInfo, nullptr, &descriptorPool);
	if (result != VK_SUCCESS) {
		Log::error("Erreur lors de la cr�ation du descriptorPool", result);
	}
}

void Vertex::createDescriptorSets(vector<VkDescriptorSet>& descriptorSets, vector<StructImageObject> imageBuffers,VkDescriptorPool* descriptorPool)
{
	
	std::vector<VkDescriptorSetLayout> layouts(swapchainObj->getSwapchainImage().size(), *descriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = *descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapchainObj->getSwapchainImage().size());
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(swapchainObj->getSwapchainImage().size());

	VkResult result = vkAllocateDescriptorSets(deviceObj->getDevice(), &allocInfo, descriptorSets.data());
	if (result != VK_SUCCESS) {
		Log::error("Erreur lors de l'allocation du descriptor sets", result);
	}

	for (size_t descriptorIndex = 0; descriptorIndex < swapchainObj->getSwapchainImage().size(); descriptorIndex++) {
		vector < VkWriteDescriptorSet> descriptorWrites;
		descriptorWrites.resize(imageBuffers.size()+1);
		//cr�� 3 descriptorSet
		//et dans 1 descriptorSet il y a 2 descripteurs, un UBO et une texture
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[descriptorIndex];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[descriptorIndex];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		for (uint32_t bindingIndex = 0; bindingIndex < imageBuffers.size(); bindingIndex++)
		{
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = imageBuffers[bindingIndex].textureImageView; //TODO -> image view
			imageInfo.sampler = textureSampler;//TODO -> texture sampler

			descriptorWrites[bindingIndex+1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[bindingIndex+1].dstSet = descriptorSets[descriptorIndex];
			descriptorWrites[bindingIndex+1].dstBinding = bindingIndex+1;
			descriptorWrites[bindingIndex+1].dstArrayElement = 0;
			descriptorWrites[bindingIndex+1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[bindingIndex+1].descriptorCount = 1;
			descriptorWrites[bindingIndex+1].pImageInfo = &imageInfo;
		}	
		

		vkUpdateDescriptorSets(
			deviceObj->getDevice(),
			static_cast<uint32_t>(descriptorWrites.size()),
			descriptorWrites.data(),
			0,
			nullptr);
	}

	

}

void Vertex::createTextureImage(const string TEXTURE_FILE_NAME, StructImageObject &imageBuffer)
{
	
	int texWidth, texHeight, texChannels;
	stbi_uc * pixels = stbi_load(TEXTURE_FILE_NAME.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	Log::message<uint64_t>(imageSize);
		if (!pixels)
		{
			Log::error("failed to load image");
		
		}

		StructBufferObject stagingBuffer;

		createBuffers(imageSize, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer.buffer,
			stagingBuffer.memory);

		void* data;
		VkResult result = vkMapMemory(deviceObj->getDevice(), stagingBuffer.memory, 0, imageSize, 0, &data);
		if (result != VK_SUCCESS)
		{
			Log::error("Failed to map memory for Image buffer", result);
		}

		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(deviceObj->getDevice(), stagingBuffer.memory);

		stbi_image_free(pixels);
		createImage(texWidth, texHeight,
			VK_FORMAT_R8G8B8A8_SRGB, 
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | 
			VK_IMAGE_USAGE_SAMPLED_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			imageBuffer);

		
		transitionImageLayout(imageBuffer.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(stagingBuffer.buffer, imageBuffer.image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		transitionImageLayout(imageBuffer.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		vkDestroyBuffer(deviceObj->getDevice(), stagingBuffer.buffer, nullptr);
		vkFreeMemory(deviceObj->getDevice(), stagingBuffer.memory, nullptr);
}

void Vertex::transitionImageLayout(VkImage image, VkFormat format,	VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = CommandBufferObj->beginSingleTimeCommands();
	
	
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	barrier.srcAccessMask = 0; // TODO
	barrier.dstAccessMask = 0; // TODO

	

	
	
		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout ==	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;		
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;		
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
			newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;		
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else 
		{
			Log::error("transition d'orgisation non support�e!");				
		}

		vkCmdPipelineBarrier(commandBuffer,	sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		Log::message<string>("pipeline BARRIER set");
		CommandBufferObj->endSingleTimeCommands(commandBuffer);
}

void Vertex::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, StructImageObject& imageStruct)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32_t>(width);
	imageInfo.extent.height = static_cast<uint32_t>(height);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0;
	VkResult error = vkCreateImage(deviceObj->getDevice(), &imageInfo, nullptr, &imageStruct.image);
	if (error != VK_SUCCESS) {
		Log::error("Failed to create imageBuffer ", error);
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(deviceObj->getDevice(), imageStruct.image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = deviceObj->findMemoryType(memRequirements.memoryTypeBits, properties);

	error = vkAllocateMemory(deviceObj->getDevice(), &allocInfo, nullptr, &imageStruct.memory);
	if (error != VK_SUCCESS) {
		Log::error("Failed to allocate memory IMAGE", error);
	}
	vkBindImageMemory(deviceObj->getDevice(), imageStruct.image, imageStruct.memory, 0);
	Log::success("Succeed to create Image !");
	createTextureImageView(imageStruct);
}

void Vertex::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
	VkCommandBuffer commandBuffer = CommandBufferObj->beginSingleTimeCommands();
	
	
	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height,1 };
	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, & region);
	Log::message("buffer copied into imageBuffer");
	CommandBufferObj->endSingleTimeCommands(commandBuffer);
}

void Vertex::createTextureImageView(StructImageObject& imageStruct)
{
	imageStruct.textureImageView = swapchainObj->createImageView(imageStruct.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	if (imageStruct.textureImageView == NULL)
	{
		Log::error("failed to create Image View for Image");

	}
	Log::success("success to create Image view for Image");
	}

void Vertex::createTextureSampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	VkResult result = vkCreateSampler(deviceObj->getDevice(), &samplerInfo, nullptr, &textureSampler);
	if (result != VK_SUCCESS) {
		Log::error("Failed to create Image Sampler", result);
	}
}

void Vertex::createDepthResources() {
	
	VkFormat depthFormat = deviceObj->findDepthFormat();
	createImage(swapchainObj->getCurrentWindowSize().width, swapchainObj->getCurrentWindowSize().height,
		depthFormat, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthBuffer);
	depthImageView = swapchainObj->createImageView(depthBuffer.image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void Vertex::loadModel(const string MODEL_PATH, vector<uint32_t> &indices, vector<vertexStruc> &vertices)
{
	std::unordered_map<vertexStruc, uint32_t> uniqueVertices{};
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
		Log::error(warn + err);
	}

	

	for (const auto& shape : shapes) {
			
		for (const auto& index : shape.mesh.indices) {
			vertexStruc vertex{};

			if (index.vertex_index >= 0)
			{
				vertex.pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};
			}
			
			if (index.texcoord_index >= 0)
			{
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}
			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			
			}
			
			indices.push_back(uniqueVertices[vertex]);
			
		}
		
	}
	uniqueVertices.clear();
	Log::message(string{ "Number of unique vertices : " }+ std::to_string(indices.size()));
	Log::message("object Loaded");
}




void Vertex::recreateVertexObj()
{
	createDepthResources();
	descriptorSetLayout = rendererObj->getDescriptorSetLayout();
	createUniformBuffers();
	CommandBufferObj->createFrameBuffer(depthImageView);
	CommandBufferObj->createCommandBuffer();
	
}

void Vertex::cleanUp()
{
	

	for (size_t i = 0; i < swapchainObj->getSwapchainImage().size(); i++) {
		vkDestroyBuffer(deviceObj->getDevice(), uniformBuffers[i], nullptr);
		vkFreeMemory(deviceObj->getDevice(), uniformBuffersMemory[i], nullptr);

	}

	


	
}

Vertex::~Vertex()
{	
	cleanUp();
	vkDestroySampler(deviceObj->getDevice(), textureSampler, nullptr);
	

	vkDestroyImageView(deviceObj->getDevice(), depthImageView, nullptr);

	vkDestroyImage(deviceObj->getDevice(), depthBuffer.image, nullptr);
	vkFreeMemory(deviceObj->getDevice(), depthBuffer.memory, nullptr);

	

	vkDestroyDescriptorSetLayout(deviceObj->getDevice(), *descriptorSetLayout, nullptr);
}