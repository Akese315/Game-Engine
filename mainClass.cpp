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
	start();
}

void mainClass::createModels()
{
	vector<string> textures = { "textures/Diffuse_2K.png" };
	createObjectInfo earth{};
	earth.FILE_NAME_OBJ = "model/earth.obj";
	earth.FILE_NAME_TEXTURE = textures;
	earth.pos = { 0.2, 0.3, 0.4 };

	earthObj = new GraphicObject(earth, deviceObj, vertexObj, renderer);
	modelList.push_back(earthObj);

	createObjectInfo floor{};
	floor.vertices = {
		{{-20.0f, -20.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{ {20.0f, -20.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{ {20.0f, 20.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		{ {-20.0f, 20.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};
	floor.indices = { 0, 1, 2, 2, 3, 0 };
	floor.FILE_NAME_TEXTURE = { "textures/grass.png" };
	floorObj = new GraphicObject(floor , deviceObj, vertexObj, renderer);
	modelList.push_back(floorObj);
}

void mainClass::createObject()
{
	object* player1 = new object("frizi", 1, earthObj);
	objectList.push_back(player1);
	object* floor = new object("floor", 0, floorObj);
	objectList.push_back(floor);
	
}

void mainClass::start()
{
	
	for (object* object : objectList)
	{
		CommandInfo command{};
		command.layoutPipeline = object->getModelObject()->getMaterial()->_layoutPipeline;
		command.graphicPipeline = object->getModelObject()->getMaterial()->_graphicPipepline;
		command.descriptorSets = object->getModelObject()->getDescriptorset();
		command.indexBuffer = object->getIndexBuffer();
		command.vertexBuffer = object->getVertexBuffer();
		command.indiceCount = object->getIndicesCount();
		commandInfoList.push_back(command);
	}

	commandBufferObj->commandBufferLoad(commandInfoList);
}

void mainClass::recreateGraphicObject()
{

	for (GraphicObject* model : modelList)
	{
		model->update();
	}
	for (object* object : objectList)
	{
		object->update();
	}
	commandInfoList.clear();
	start();
}

mainClass::~mainClass()
{
	for (size_t i = 0; i < objectList.size(); i++)
	{
		delete objectList[i];
	}
	objectList.clear();

	for (size_t i = 0; i < modelList.size(); i++)
	{
		delete modelList[i];
	}
	modelList.clear();
}