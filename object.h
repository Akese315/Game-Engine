#pragma once
#define GLM_FORCE_RADIANS
#include "Type.h"
#include "GraphicObject.h"




class object
{
public:
	object(string name,uint32_t id, GraphicObject* graphicObj);
	~object();
	void init();
	void move(glm::vec3 direction, glm::vec1 vitesse, int forward);
	position getPosition();
	array<StructBufferObject, 2> getBufferStruct();
private:
	string name;
	uint32_t id;
	GraphicObject* graphicObj;
	StructBufferObject vertexBufferStruct;
	StructBufferObject indexBufferStruct;
	vector<vertexStruc> vertices;
	vector<uint32_t> indices;
};

