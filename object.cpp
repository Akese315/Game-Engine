#include "object.h"

object::object(string name, uint32_t id, GraphicObject* graphicObj)
{
	this->name = name;
	this->id = id;
	this->graphicObj = graphicObj;

	vertices = graphicObj->getVertices(vertexBufferStruct);
	indices = graphicObj->getIndices(indexBufferStruct);
}
/*
StructBufferObject* object::getBufferStruct()
{
	StructBufferObject buffer[2];
	buffer[0] = indexBufferStruct;
	buffer[1] = vertexBufferStruct;

	//array<StructBufferObject, 2> bufferStruct = { indexBufferStruct, vertexBufferStruct };
	return buffer;
}*/
void object::move(glm::vec3 direction, glm::vec1 vitesse, int forward, float time)
{	
	
	updateVertices(vertices, direction, time);
	graphicObj->updateBuffer(vertexBufferStruct, indexBufferStruct, vertices, indices);
}

void object::updateVertices(vector<vertexStruc>& vertexList, glm::vec3 direction, float time)
{
	for (uint32_t i = 0; i < vertexList.size(); i++)
	{
		
		
		if (direction.x != 0)
		{
			vertexList[i].pos.x += direction.x * time;
		}
		if (direction.y != 0)
		{
			vertexList[i].pos.y += direction.y * time;
		}
		if (direction.z != 0)
		{
			vertexList[i].pos.z += direction.z * time;
		}	
	}
	
}

GraphicObject* object::getModelObject()
{
	return graphicObj;
}
VkBuffer* object::getIndexBuffer()
{
	return &indexBufferStruct.buffer;
}
VkBuffer* object::getVertexBuffer()
{
	return &vertexBufferStruct.buffer;
}
uint32_t object::getIndicesCount()
{
	return indices.size();
}
void object::update()
{
	graphicObj->cleanup(indexBufferStruct, vertexBufferStruct);
	graphicObj->getVertices(vertexBufferStruct);
	graphicObj->getIndices(indexBufferStruct);
	move(glm::vec3(4, 4, 4), glm::vec1(1), 1, 1.0);
	
}
object::~object()
{
	vertices.clear();
	indices.clear();
	graphicObj->cleanup(indexBufferStruct, vertexBufferStruct);
}