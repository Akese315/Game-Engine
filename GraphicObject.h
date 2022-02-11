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
	GraphicObject(const createObjectInfo objectInfo,  Device* deviceObj, Vertex* vertexObj, vulkan_render* renderer, CommandBuffer* commandBufferObj);
	~GraphicObject();
	void update();
	void cleanup(StructBufferObject indexBufferStruct, StructBufferObject vertexBufferStruct);
	vector<VkDescriptorSet>* getDescriptorset();
	uint32_t getObjectNumber();
	MaterialStruct* getMaterial();
	vector<uint32_t> getIndices(StructBufferObject& indexBufferStruct);
	vector<vertexStruc> getVertices(StructBufferObject& vertexBufferStruct);
	void updateBuffer(StructBufferObject vertexStructbuffer, StructBufferObject indexStructbuffer, vector<vertexStruc> vertexStruct, vector<uint32_t> indexStruct);
	
private:
	MaterialStruct currentMaterial;
	vector<vertexStruc> vertices;
	vector<uint32_t> indices;
	vector<VkDescriptorSet> descriptorSets;
	VkDescriptorPool descriptorPool;
	vector<StructImageObject> imageBuffers;

	Vertex* vertexObj;
	Device* deviceObj;
	CommandBuffer* commandBufferObj;
	vulkan_render* rendererObj;
	bool checkModel(string FILE_NAME_OBJ);
	


};
