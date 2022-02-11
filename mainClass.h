#pragma once
#include "type.h"
#include "Device.h"
#include "Vertex.h"
#include "CommandBuffer.h"
#include "vulkan_render.h"
#include "GraphicObject.h"
#include "object.h"
#include "floor.h"
#include "WindowEvent.h"
#include <chrono>

class mainClass
{
public:
	mainClass(Device* deviceObj, Vertex* vertexObj, CommandBuffer* commandBufferObj, vulkan_render* renderer, WindowEvent* WindowEventObj);
	~mainClass();
	
	void recreateGraphicObject();
	void update();

private:
	Device* deviceObj;
	Vertex* vertexObj;
	CommandBuffer* commandBufferObj;
	vulkan_render* renderer;
	WindowEvent* WindowEventObj;
	vector<object*> objectList;
	vector<GraphicObject*> modelList;
	vector<CommandInfo> commandInfoList;
	GraphicObject* earthObj;
	GraphicObject* floorObj;
	object* player1;
	
	std::chrono::high_resolution_clock::time_point time;
	std::chrono::high_resolution_clock::time_point LastTime;
	uint16_t FPS;
	int nb_second;
	float movementTime;

	void init();
	void createModels();
	void createObject();
	void createCamera();
	void showFPS(string ms);
	void updateMovement(float time);
	std::chrono::high_resolution_clock::time_point getTime();
	void start();
};

