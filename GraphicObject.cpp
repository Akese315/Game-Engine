#include "GraphicObject.h"

GraphicObject::GraphicObject(const createObjectInfo objectInfo, Device* deviceObj, Vertex* vertexObj, vulkan_render* renderer)
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

void GraphicObject::updateBuffer(VkBuffer vertex,VkBuffer index)
{
	/*
	void* data;
	vkMapMemory(deviceObj->getDevice(), index, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(deviceObj->getDevice(), stagingBuffer.memory);*/
}

uint32_t GraphicObject::getObjectNumber()
{
	return 0;
}

MaterialStruct* GraphicObject::getMaterial()
{
	return &currentMaterial;
}
vector<uint32_t> GraphicObject::getIndices(StructBufferObject& indexBufferStruct)
{
	this->vertexObj->createIndexBuffer(indexBufferStruct, indices);
	return indices;
}
vector<vertexStruc> GraphicObject::getVertices(StructBufferObject& vertexBufferStruct)
{
	
	this->vertexObj->createVertexBuffer(vertexBufferStruct, vertices);
	return vertices;
	
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
	// cette fonction est appellé uniquement à la fin et par l'objet lui même
	vkDestroyBuffer(deviceObj->getDevice(), indexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), indexBufferStruct.memory, nullptr);

	vkDestroyBuffer(deviceObj->getDevice(), vertexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), vertexBufferStruct.memory, nullptr);
	
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