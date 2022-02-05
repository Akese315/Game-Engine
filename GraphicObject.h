#pragma once
#include "Log.h"
#include "Vertex.h"
#include "Type.h"

class GraphicObject
{
public:
	GraphicObject(const createObjectInfo objectInfo, Vertex* vertexObj, CommandBuffer* CommandBufferObj, Device* deviceObj);
	~GraphicObject();



private:
	Vertex *vertexObj;
	Device *deviceObj;
	CommandBuffer* CommandBufferObj;
	vulkan_render* rendererObj;
	string OBJ_FILE_NAME;
	vector<string> TEXTURE_FILE_NAME;
	StructBufferObject vertexBufferStruct;
	StructBufferObject indexBufferStruct;
	vector<StructImageObject> imageBuffers;
	vector<vertexStruc> vertices;
	vector<uint32_t> indices;
	vector<VkDescriptorSet> descriptorSets;
	

	struct position
	{
		float x;
		float y;
		float z;
	}position;
	
};

