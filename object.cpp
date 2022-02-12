#include "object.h"

object::object(string name, uint32_t id, GraphicObject* graphicObj, glm::vec3 position)
{
	this->name = name;
	this->id = id;
	this->graphicObj = graphicObj;
	this->position = position;
	graphicObj->getVertices(vertices);
	graphicObj->getIndices(indices);
	updateVertices(vertices,position, 1.0f);
	graphicObj->createIndexBuffer(indexBufferStruct,indices);
	graphicObj->createVertexBuffer(vertexBufferStruct,vertices);
	graphicObj->updateBuffer(vertexBufferStruct, indexBufferStruct, vertices, indices);
}


void object::move(glm::vec3 direction,float vitesse, float time)
{	
	position.x += direction.x * time;
	position.y += direction.y * time;
	position.z += direction.z * time;
	string positionString = "position = (" + to_string(position.x)+", " + to_string(position.y)+", " + to_string(position.z)+")";
	Log::message(positionString);
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

glm::vec3 object::getPosition()
{
	return position;
}

void object::setPosition(glm::vec3 position)
{
	this->position = position;
}
void object::update()
{
	graphicObj->cleanup(indexBufferStruct, vertexBufferStruct);
	graphicObj->createIndexBuffer(indexBufferStruct, indices);
	graphicObj->createVertexBuffer(vertexBufferStruct, vertices);
	
}

object::~object()
{
	vertices.clear();
	indices.clear();
	graphicObj->cleanup(indexBufferStruct, vertexBufferStruct);
}