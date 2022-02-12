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
	// à chaque fois que la fonction draw est exécutée, elle appelle cette fonction qui en fonction 
		// du temps qui s'est écoulé depuis le début tourne tant le model sur un axe et une position donnée

	if (rotation != 0.0f)
	{
		uniformBuffer.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		uniformBuffer.model = glm::mat4(1.0f);
	}
	
	//le premier paramètre indique le model, le deuxième permet de faire une rotation de 90° en fonction du temps
	// et le dernier permet d'indiquer l'axe de rotation
	uniformBuffer.view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 10.0f),// le premier paramètre : la position de l'oeil(caméra)
		glm::vec3(0.0f, 0.0f, 0.0f),//le deuxième : la position de la cible (ce que l'on regarde) inclinaison
		glm::vec3(0.0f, 1.0f, 0.0f));//le dernier est l'axe de la hauteur



	uniformBuffer.proj = glm::perspective(glm::radians(60.0f), swapchainObj->getCurrentWindowSize().width / (float)swapchainObj->getCurrentWindowSize().height, 0.1f, 10.0f);
	//donne la perspective ici de 45° et les deux autre paramètre sont le ration de la taille de l'écran (ici)
	uniformBuffer.proj[1][1] *= -1;

	//la bibliothèque glm a été crée pour OPENGL qui inverse le positif et le négatif donc il faut tout inverser 
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