#include "camera.h"
camera::camera(Vertex* vertexObj, SwapChain* swapchainObj, Device* deviceObj)
{
	this->vertexObj = vertexObj;
	this->swapchainObj = swapchainObj;
	this->deviceObj = deviceObj;
	uniformBufferStruct = vertexObj->createUniformBuffers();
}

void camera::recreate()
{
	uniformBufferStruct = vertexObj->createUniformBuffers();
}

void camera::update(uint16_t imageIndex, float rotation,float time)
{
	// � chaque fois que la fonction draw est ex�cut�e, elle appelle cette fonction qui en fonction 
		// du temps qui s'est �coul� depuis le d�but tourne tant le model sur un axe et une position donn�e

	if (rotation != 0.0f)
	{
		uniformBuffer.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		uniformBuffer.model = glm::mat4(1.0f);
	}
	
	//le premier param�tre indique le model, le deuxi�me permet de faire une rotation de 90� en fonction du temps
	// et le dernier permet d'indiquer l'axe de rotation
	uniformBuffer.view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 10.0f),// le premier param�tre : la position de l'oeil(cam�ra)
		glm::vec3(0.0f, 0.0f, 0.0f),//le deuxi�me : la position de la cible (ce que l'on regarde) inclinaison
		glm::vec3(0.0f, 1.0f, 0.0f));//le dernier est l'axe de la hauteur



	uniformBuffer.proj = glm::perspective(glm::radians(60.0f), swapchainObj->getCurrentWindowSize().width / (float)swapchainObj->getCurrentWindowSize().height, 0.1f, 10.0f);
	//donne la perspective ici de 45� et les deux autre param�tre sont le ration de la taille de l'�cran (ici)
	uniformBuffer.proj[1][1] *= -1;

	//la biblioth�que glm a �t� cr�e pour OPENGL qui inverse le positif et le n�gatif donc il faut tout inverser 
	void* data;
	vkMapMemory(deviceObj->getDevice(), uniformBufferStruct->at(imageIndex).memory, 0, sizeof(uniformBuffer), 0, &data);
	memcpy(data, &uniformBuffer, sizeof(uniformBuffer));
	vkUnmapMemory(deviceObj->getDevice(), uniformBufferStruct->at(imageIndex).memory);
}
void camera::cleanUp()
{
	for (size_t i = 0; i < swapchainObj->getSwapchainImage().size(); i++) {
		vkDestroyBuffer(deviceObj->getDevice(), uniformBufferStruct->at(i).buffer, nullptr);
		vkFreeMemory(deviceObj->getDevice(), uniformBufferStruct->at(i).memory, nullptr);
	}
}

camera::~camera()
{
	cleanUp();
}