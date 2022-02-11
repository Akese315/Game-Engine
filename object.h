#pragma once
#define GLM_FORCE_RADIANS
#include "Type.h"
#include "GraphicObject.h"




class object
{
public:
	object(string name,uint32_t id, GraphicObject* graphicObj);
	~object();
	void move(glm::vec3 direction, glm::vec1 vitesse, int forward, float time);
	position getPosition();
	GraphicObject* getModelObject();
	VkBuffer* getVertexBuffer();
	VkBuffer* getIndexBuffer();
	uint32_t getIndicesCount();
	void updateVertices(vector<vertexStruc>& vertexList, glm::vec3 direction, float time);
	void updateIndices(vector<uint32_t>& indexList);
	//StructBufferObject* getBufferStruct();
	void update();
private:
	string name;
	uint32_t id;
	GraphicObject* graphicObj;
	StructBufferObject vertexBufferStruct;
	StructBufferObject indexBufferStruct;
	vector<vertexStruc> vertices;
	vector<uint32_t> indices;
};

