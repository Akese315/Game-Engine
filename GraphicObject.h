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
	void load(StructBufferObject &indexBufferStruct, StructBufferObject &vertexBufferStruct,
		vector<vertexStruc>& vertices, vector<uint32_t>& indices);
	void update(StructBufferObject& indexBufferStruct, StructBufferObject& vertexBufferStruct,
		 vector<vertexStruc>& vertices, vector<uint32_t>& indices);
	void cleanup(StructBufferObject indexBufferStruct, StructBufferObject vertexBufferStruct);
	uint32_t getObjectNumber();
	vector<uint32_t> getIndices();
	vector<vertexStruc> getVertices();
	
private:

	
	
	vector<vertexStruc> vertices;
	vector<uint32_t> indices;
	vector<VkDescriptorSet> descriptorSets;
	Vertex* vertexObj;
	Device* deviceObj;
	vector<StructImageObject> imageBuffers;
	CommandBuffer* CommandBufferObj;
	vulkan_render* rendererObj;
	uint32_t incrementCount();
	std::string getOBJ_FILE_NAME();
	bool checkModel(createObjectInfo objectInfo);			
	


};
