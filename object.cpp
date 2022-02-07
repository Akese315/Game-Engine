#include "object.h"

object::object(string name, uint32_t id, GraphicObject* graphicObj)
{
	this->name = name;
	this->id = id;
	this->graphicObj = graphicObj;
	
	init();
}

void object::init()
{
	vertices = graphicObj->getVertices();
	indices = graphicObj->getIndices();
	graphicObj->load(indexBufferStruct ,vertexBufferStruct,vertices, indices);
}

array<StructBufferObject,2>object::getBufferStruct()
{
	array<StructBufferObject, 2> bufferStruct = { indexBufferStruct, vertexBufferStruct };
	return bufferStruct;
}
void object::move(glm::vec3 direction, glm::vec1 vitesse, int forward)
{

}
object::~object()
{
	vertices.clear();
	indices.clear();
	graphicObj->cleanup(indexBufferStruct,vertexBufferStruct);
}