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
void object::move(glm::vec3 direction, glm::vec1 vitesse, int forward)
{
	
	

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
	vertices = graphicObj->getVertices(vertexBufferStruct);
	indices = graphicObj->getIndices(indexBufferStruct);
}
object::~object()
{
	vertices.clear();
	indices.clear();
	graphicObj->cleanup(indexBufferStruct,vertexBufferStruct);
}