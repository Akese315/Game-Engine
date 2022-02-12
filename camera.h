#pragma once
#include <vulkan/include/vulkan.h>
#include "Log.h"
#include <glm/glm.hpp>
#include "Vertex.h"

class camera
{
private:
	UniformBufferObject uniformBuffer;

	Vertex* vertexObj;
	SwapChain* swapchainObj;
	Device* deviceObj;

	vector<StructBufferObject>* uniformBufferStruct;

public:
	camera(Vertex* vertexObj, SwapChain* swapchainObj, Device* deviceObj);
	~camera();
	void recreate();
	void cleanUp();
	void update(uint16_t imageIndex, float rotation, float time);
};


