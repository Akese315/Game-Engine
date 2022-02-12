#include "GraphicObject.h"

GraphicObject::GraphicObject(const createObjectInfo objectInfo, Device* deviceObj, Vertex* vertexObj, vulkan_render* renderer, CommandBuffer* commandBufferObj)
{
	if (!objectInfo.indices.empty()&& !objectInfo.indices.empty())
	{
		vertices = objectInfo.vertices;
		indices = objectInfo.indices;
	}
	else if (!objectInfo.FILE_NAME_OBJ.empty())
	{		
		 bool uniqueModel = checkModel(objectInfo.FILE_NAME_OBJ);
		 if (!uniqueModel)
		 {
			 return;
		 }
		this->vertexObj->loadModel(objectInfo.FILE_NAME_OBJ, indices, vertices);
	}
	else
	{
		Log::error("Failed to load 3D OBJECT : no file");
		return;
	}	
	/*
	On vérifie si des données en dur simple ne sont pas déjà écrites sinon on regarde si un fichier.obj n'est pas donné
	Et si c'est le cas on le charge, sinon on return un objet qui n'a rien.*/
	this->rendererObj = renderer;
	currentMaterial = rendererObj->getNormalMaterial();
	this->vertexObj = vertexObj;
	this->commandBufferObj = commandBufferObj;
	this->deviceObj = deviceObj;

	//initialisation des objets de bases à cet endroit pour éviter si jamais il y a un
	//double de model de modifier les variables globales déjà initialisé.
	
	if (objectInfo.FILE_NAME_TEXTURE.size() == 0)
	{
		Log::message("the object has no texture");
	}
	else
	{
		imageBuffers.resize(objectInfo.FILE_NAME_TEXTURE.size());

		for (size_t i = 0; i < objectInfo.FILE_NAME_TEXTURE.size(); i++)
		{
			this->vertexObj->createTextureImage(objectInfo.FILE_NAME_TEXTURE[i], imageBuffers[i]);
		}
	}	
	/*
	Si il n'y a pas de fichier texture alors on ne fait rien et on passe à la suite.
	Sinon on adapte la taille d "imageBuffers" à celle du nombre de texture et on va 
	les chargé dans des buffers	
	*/
	this->vertexObj->createDescriptorPool(descriptorPool);
	this->vertexObj->createDescriptorSets(descriptorSets, imageBuffers,&descriptorPool);
	for (int i = 0; i < descriptorSets.size(); i++)
	{
		Log::message(&descriptorSets[i]);
	}
	//Remplissage des descriptors sets
}

bool GraphicObject::checkModel(string FILE_NAME_OBJ)
{
	static unordered_map<string, uint32_t> objectInfoMAP;
	static uint32_t count = 0;
	if (objectInfoMAP.count(FILE_NAME_OBJ) == 0)
	{
		objectInfoMAP[FILE_NAME_OBJ] = count;
		count++;
		return true;
	}
	return false;
}

void GraphicObject::updateBuffer(StructBufferObject vertexStructbuffer, StructBufferObject indexStructbuffer,vector<vertexStruc> vertexStruct, vector<uint32_t> indexStruct)
{
	/*
	VkDeviceSize bufferSizeIndex = sizeof(indexStruct[0]) * indexStruct.size();
	memcpy(indexStructbuffer.data, indexStruct.data(), (size_t)bufferSizeIndex);
	commandBufferObj->copyBuffer(&indexStructbuffer, bufferSizeIndex);
	*/
	VkDeviceSize bufferSizeVertex = sizeof(vertexStruct[0]) * vertexStruct.size();
	memcpy(vertexStructbuffer.data, vertexStruct.data(), (size_t)bufferSizeVertex);
	commandBufferObj->copyBuffer(&vertexStructbuffer, bufferSizeVertex);
	
}

uint32_t GraphicObject::getObjectNumber()
{
	return 0;
}

MaterialStruct* GraphicObject::getMaterial()
{
	return &currentMaterial;
}
void GraphicObject::getIndices(vector<uint32_t>& ObjectIndices)
{
	if (ObjectIndices.empty())
	{
		ObjectIndices = indices;
	}
	
}
void GraphicObject::getVertices(vector<vertexStruc>& ObjectVertices)
{
	if (ObjectVertices.empty())
	{
		ObjectVertices = vertices;
	}
	
}
void GraphicObject::createVertexBuffer(StructBufferObject& vertexBufferStruct, vector<vertexStruc>& ObjectVertices)
{
	VkDeviceSize bufferSize = sizeof(ObjectVertices[0]) * ObjectVertices.size();


	vertexObj->createBuffers(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBufferStruct.stagingBuffer, vertexBufferStruct.stagingMemory);

	vkMapMemory(deviceObj->getDevice(), vertexBufferStruct.stagingMemory, 0, bufferSize, 0, &vertexBufferStruct.data);
	memcpy(vertexBufferStruct.data, ObjectVertices.data(), (size_t)bufferSize);
	//vkUnmapMemory(deviceObj->getDevice(), vertexBufferStruct.stagingMemory);//à la fin uniquement

	vertexObj->createBuffers(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferStruct.buffer,
		vertexBufferStruct.memory);

	commandBufferObj->copyBuffer(&vertexBufferStruct, bufferSize);
}
void GraphicObject::createIndexBuffer(StructBufferObject& indexBufferStruct, vector<uint32_t>& ObjectIndices)
{
	VkDeviceSize bufferSize = sizeof(ObjectIndices[0]) * ObjectIndices.size();

	vertexObj->createBuffers(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexBufferStruct.stagingBuffer,
		indexBufferStruct.stagingMemory);

	vkMapMemory(deviceObj->getDevice(), indexBufferStruct.stagingMemory, 0, bufferSize, 0, &indexBufferStruct.data);
	memcpy(indexBufferStruct.data, ObjectIndices.data(), (size_t)bufferSize);
	//vkUnmapMemory(deviceObj->getDevice(), stagingBuffer.memory); on unMap uniquement à la fin

	vertexObj->createBuffers(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferStruct.buffer,
		indexBufferStruct.memory);
	commandBufferObj->copyBuffer(&indexBufferStruct, bufferSize);
}
vector<VkDescriptorSet>* GraphicObject::getDescriptorset()
{
	return &descriptorSets;
}
void GraphicObject::update()
{
	//destruction de la pool et du descriptorset
	vkDestroyDescriptorPool(deviceObj->getDevice(), descriptorPool, nullptr);
	descriptorSets.clear();
	descriptorSets.resize(0);
	//récupération du material :: faire toujours attention 
	currentMaterial = rendererObj->getNormalMaterial(); 
	//reconstruction des descriptorset et de la pool
	this->vertexObj->createDescriptorPool(descriptorPool);
	this->vertexObj->createDescriptorSets(descriptorSets, imageBuffers, &descriptorPool);
	for (int i = 0; i < descriptorSets.size(); i++)
	{
		Log::message(descriptorSets[i]);
	}
}

void GraphicObject::cleanup(StructBufferObject indexBufferStruct, StructBufferObject vertexBufferStruct)
{
	// cette fonction est appellé par l'objet lui même
	vkDestroyBuffer(deviceObj->getDevice(), indexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), indexBufferStruct.memory, nullptr);

	vkDestroyBuffer(deviceObj->getDevice(), vertexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), vertexBufferStruct.memory, nullptr);

	vkUnmapMemory(deviceObj->getDevice(), indexBufferStruct.stagingMemory);
	vkUnmapMemory(deviceObj->getDevice(), vertexBufferStruct.stagingMemory);

	vkDestroyBuffer(deviceObj->getDevice(), indexBufferStruct.stagingBuffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), indexBufferStruct.stagingMemory, nullptr);
	
	vkDestroyBuffer(deviceObj->getDevice(), vertexBufferStruct.stagingBuffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), vertexBufferStruct.stagingMemory, nullptr);

	
}

GraphicObject::~GraphicObject()
{

	vkDestroyDescriptorPool(deviceObj->getDevice(), descriptorPool, nullptr);
	vertices.clear();
	indices.clear();
	for (size_t i = 0; i < imageBuffers.size(); i++)
	{
		vkDestroyImageView(deviceObj->getDevice(), imageBuffers[i].textureImageView, nullptr);
		vkDestroyImage(deviceObj->getDevice(), imageBuffers[i].image, nullptr);
		vkFreeMemory(deviceObj->getDevice(), imageBuffers[i].memory, nullptr);
	}	
}