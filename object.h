#pragma once
#define GLM_FORCE_RADIANS
#include "Type.h"
#include "GraphicObject.h"




class object : public GraphicObject
{
public:
	object(string name, const createObjectInfo objectInfo, Device* deviceObj, Vertex* vertexObj, CommandBuffer* commandBufferObj, vulkan_render* renderer) :GraphicObject(objectInfo,  deviceObj, vertexObj, commandBufferObj, renderer) {};
	
	~object();
	void move(glm::vec3 direction, glm::vec1 vitesse, int forward);
	position getPosition();
private:
	
};

