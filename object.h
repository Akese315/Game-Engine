#pragma once
#define GLM_FORCE_RADIANS
#include "Type.h"
#include "GraphicObject.h"




class object
{
public:
	object(string name,uint32_t id, GraphicObject* graphicObj, glm::vec3 position);
	~object();
	void move(glm::vec3 direction, float vitesse, float time);
	glm::vec3 getPosition();
	GraphicObject* getModelObject();
	VkBuffer* getVertexBuffer();
	VkBuffer* getIndexBuffer();
	uint32_t getIndicesCount();
	void setPosition(glm::vec3 position);
	void updateVertices(vector<vertexStruc>& vertexList, glm::vec3 direction, float time);
	void updateIndices(vector<uint32_t>& indexList);
	void update();
private:
	string name;
	uint32_t id;
	GraphicObject* graphicObj;
	StructBufferObject vertexBufferStruct;
	StructBufferObject indexBufferStruct;
	vector<vertexStruc> vertices;
	vector<uint32_t> indices;
	glm::vec3 position;
};

