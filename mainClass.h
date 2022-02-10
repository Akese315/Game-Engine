#pragma once
#include "type.h"
#include "Device.h"
#include "Vertex.h"
#include "CommandBuffer.h"
#include "vulkan_render.h"
#include "GraphicObject.h"
#include "object.h"
#include "floor.h"

class mainClass
{
public:
	mainClass(Device* deviceObj, Vertex* vertexObj, CommandBuffer* commandBufferObj, vulkan_render* renderer);
	~mainClass();
	void init();
	void createModels();
	void createObject();
	void start();
	void recreateGraphicObject();
private:
	Device* deviceObj;
	Vertex* vertexObj;
	CommandBuffer* commandBufferObj;
	vulkan_render* renderer;
	vector<object*> objectList;
	vector<GraphicObject*> modelList;
	vector<CommandInfo> commandInfoList;
	GraphicObject* earthObj;
	GraphicObject* floorObj;
};

