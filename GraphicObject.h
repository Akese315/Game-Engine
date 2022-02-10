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
	GraphicObject(const createObjectInfo objectInfo,  Device* deviceObj, Vertex* vertexObj, vulkan_render* renderer);
	~GraphicObject();
	void update();
	void cleanup(StructBufferObject indexBufferStruct, StructBufferObject vertexBufferStruct);
	vector<VkDescriptorSet>* getDescriptorset();
	uint32_t getObjectNumber();
	MaterialStruct* getMaterial();
	vector<uint32_t> getIndices(StructBufferObject& indexBufferStruct);
	vector<vertexStruc> getVertices(StructBufferObject& vertexBufferStruct);
	void updateBuffer(VkBuffer vertex, VkBuffer index);
	
private:
	MaterialStruct currentMaterial;
	vector<vertexStruc> vertices;
	vector<uint32_t> indices;
	vector<VkDescriptorSet> descriptorSets;
	VkDescriptorPool descriptorPool;
	Vertex* vertexObj;
	Device* deviceObj;
	vector<StructImageObject> imageBuffers;	
	vulkan_render* rendererObj;
	bool checkModel(string FILE_NAME_OBJ);
	


};
