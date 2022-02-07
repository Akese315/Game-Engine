#include "GraphicObject.h"

GraphicObject::GraphicObject(const createObjectInfo objectInfo, Device* deviceObj, Vertex* vertexObj, CommandBuffer* commandBufferObj, vulkan_render* renderer)
{
	if (objectInfo.FILE_NAME_OBJ.empty())
		{
		Log::error("Failed to load 3D OBJECT : no file");
		return;
		}
	bool uniqueModel = checkModel(objectInfo);
	if (!uniqueModel)
	{
		this->rendererObj = nullptr;
		this->vertexObj = nullptr;
		this->deviceObj = nullptr;
		this->CommandBufferObj = nullptr;
		return;
	}
	this->rendererObj = renderer;
	this->vertexObj = vertexObj;
	this->deviceObj = deviceObj;
	this->CommandBufferObj = commandBufferObj;

	this->vertexObj->loadModel(objectInfo.FILE_NAME_OBJ, indices, vertices);
	if (objectInfo.FILE_NAME_TEXTURE.size() == 0)
	{
		Log::message("the object has no texture");
	}
	imageBuffers.resize(objectInfo.FILE_NAME_TEXTURE.size());
	for (size_t i = 0; i < objectInfo.FILE_NAME_TEXTURE.size(); i++)
	{
		this->vertexObj->createTextureImage(objectInfo.FILE_NAME_TEXTURE[i], imageBuffers[i]);
	}
	this->vertexObj->createDescriptorSets(descriptorSets, imageBuffers);
}

bool GraphicObject::checkModel(createObjectInfo objectInfo)
{
	static unordered_map<string, uint32_t> objectInfoMAP;
	static uint32_t count = 0;
	if (objectInfoMAP.count(objectInfo.FILE_NAME_OBJ) == 0)
	{
		objectInfoMAP[objectInfo.FILE_NAME_OBJ] = count;
		count++;
		return true;
	}
	return false;
}
void GraphicObject::load(StructBufferObject& indexBufferStruct, StructBufferObject& vertexBufferStruct,
	vector<vertexStruc>& vertices, vector<uint32_t>& indices)
{
	this->vertexObj->createIndexBuffer(indexBufferStruct, indices);
	this->vertexObj->createVertexBuffer(vertexBufferStruct, vertices);
	cout <<"indices : " << &indices << endl;
	CommandBufferObj->commandBufferLoad(rendererObj->getRenderPass(),
		rendererObj->getGraphicsPipeline(),
		rendererObj->getPipelineLayout(),
		&vertexBufferStruct.buffer,
		&indexBufferStruct.buffer,
		&indices, descriptorSets);
}

uint32_t GraphicObject::getObjectNumber()
{
	return 0;
}

vector<uint32_t> GraphicObject::getIndices()
{
	return indices;
}
vector<vertexStruc> GraphicObject::getVertices()
{
	return vertices;
}
void GraphicObject::update(StructBufferObject& indexBufferStruct, StructBufferObject &vertexBufferStruct, vector<vertexStruc>& vertices, vector<uint32_t>& indices)
{
	load( indexBufferStruct,  vertexBufferStruct, vertices, indices);
}

void GraphicObject::cleanup(StructBufferObject indexBufferStruct, StructBufferObject vertexBufferStruct)
{
	vkDestroyBuffer(deviceObj->getDevice(), indexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), indexBufferStruct.memory, nullptr);

	vkDestroyBuffer(deviceObj->getDevice(), vertexBufferStruct.buffer, nullptr);
	vkFreeMemory(deviceObj->getDevice(), vertexBufferStruct.memory, nullptr);
}
GraphicObject::~GraphicObject()
{
	vertices.clear();
	indices.clear();
	for (size_t i = 0; i < imageBuffers.size(); i++)
	{
		vkDestroyImageView(deviceObj->getDevice(), imageBuffers[i].textureImageView, nullptr);
		vkDestroyImage(deviceObj->getDevice(), imageBuffers[i].image, nullptr);
		vkFreeMemory(deviceObj->getDevice(), imageBuffers[i].memory, nullptr);
	}	
}