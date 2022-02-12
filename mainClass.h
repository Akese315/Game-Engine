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
#include "camera.h"
#include "balle.h"

class mainClass
{
public:
	mainClass(Device* deviceObj, Vertex* vertexObj, CommandBuffer* commandBufferObj, vulkan_render* renderer, WindowEvent* WindowEventObj, SwapChain* swapchainObj);
	~mainClass();
	
	void recreateGraphicObject();
	void cleanup();
	void update(uint16_t imageIndex);

private:
	Device* deviceObj;
	Vertex* vertexObj;
	CommandBuffer* commandBufferObj;
	vulkan_render* renderer;
	WindowEvent* WindowEventObj;
	camera* cameraObj;
	SwapChain* swapchainObj;

	vector<object*> objectList;
	vector<GraphicObject*> modelList;
	vector<CommandInfo> commandInfoList;
	GraphicObject* earthObj;
	GraphicObject* earth_polygonObj;
	GraphicObject* floorObj;
	balle* player1;
	balle* player2;
	
	std::chrono::high_resolution_clock::time_point timeAtFrame;
	std::chrono::high_resolution_clock::time_point LastTime;
	uint16_t FPS;
	int nb_second;
	float movementTime;

	void init();
	void createModels();
	void createObject();
	void createCamera();
	void showFPS(string ms, float currentTime);
	void updateMovement(float time);
	std::chrono::high_resolution_clock::time_point getTime();
	void start();
};

