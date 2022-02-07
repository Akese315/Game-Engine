#include "mainClass.h"
mainClass::mainClass(Device* deviceObj, Vertex* vertexObj, CommandBuffer* commandBufferObj, vulkan_render* renderer)
{
	this->deviceObj = deviceObj;
	this->vertexObj = vertexObj;
	this->commandBufferObj =commandBufferObj;
	this->renderer = renderer;

	init();
}

void mainClass::init()
{
	
	createModels();
	createObject();
}

void mainClass::createModels()
{
	vector<string> textures = { "textures/Diffuse_2K.png" };
	createObjectInfo objectInfo{};
	objectInfo.FILE_NAME_OBJ = "model/earth.obj";
	objectInfo.FILE_NAME_TEXTURE = textures;
	objectInfo.pos = { 0.2, 0.3, 0.4 };

	player = new GraphicObject(objectInfo, deviceObj, vertexObj, commandBufferObj, renderer);
	modelList.push_back(player);
}

void mainClass::createObject()
{
	object* player1 = new object("frizi", 1, player);
	objectList.push_back(player1);
}

void mainClass::cleanUp()
{
	for (size_t i = 0; i < objectList.size(); i++)
	{
		delete objectList[i];
	}
	objectList.clear();
	
}

void mainClass::recreateGraphicObject()
{
	createObject();
}

mainClass::~mainClass()
{
	cleanUp();
	for (size_t i = 0; i < modelList.size(); i++)
	{
		delete modelList[i];
	}
	modelList.clear();
}