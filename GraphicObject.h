#pragma once
#include "Log.h"
#include "Vertex.h"
#include "Device.h"
#include "vulkan_render.h"
#include "type.h"
#include <unordered_map>
#include <glm/gtx/hash.hpp>

class GraphicObject

{
public:
	GraphicObject(const createObjectInfo objectInfo,  Device* deviceObj, Vertex* vertexObj, CommandBuffer* commandBufferObj, vulkan_render* renderer);
	~GraphicObject();
	
	static vector<VkDescriptorSet> descriptorSets;
	static string OBJ_FILE_NAME;
	static vector<string> TEXTURE_FILE_NAME;
	static vector<StructImageObject> imageBuffers;
private:

	
	vector<vertexStruc> vertices;
	vector<uint32_t> indices;
	StructBufferObject vertexBufferStruct;
	StructBufferObject indexBufferStruct;
	position pos;

	Vertex* vertexObj;
	Device* deviceObj;
	CommandBuffer* CommandBufferObj;
	vulkan_render* rendererObj;
	uint32_t incrementCount();

	void update();		
	void cleanup();


};

