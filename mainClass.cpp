#include "mainClass.h"
#include <math.h>
mainClass::mainClass(Device* deviceObj, Vertex* vertexObj, CommandBuffer* commandBufferObj, vulkan_render* renderer, WindowEvent* WindowEventObj, SwapChain* swapchainObj)
{
	this->deviceObj = deviceObj;
	this->vertexObj = vertexObj;
	this->commandBufferObj =commandBufferObj;
	this->renderer = renderer;
	this->WindowEventObj = WindowEventObj;
	this->swapchainObj = swapchainObj;

	FPS = 0;
	nb_second = 1;
	srand(time(NULL));
	init();
}

void mainClass::init()
{
	createCamera();
	createModels();
	createObject();
	start();
}

void mainClass::createModels()
{
	vector<string> textures = { "textures/Diffuse_2K.png" };
	createObjectInfo earth{};
	earth.FILE_NAME_OBJ = "model/earth.obj";
	earth.FILE_NAME_TEXTURE = textures;
	earthObj = new GraphicObject(earth, deviceObj, vertexObj, renderer, commandBufferObj);
	modelList.push_back(earthObj);

}

void mainClass::createObject()
{
	player1 = new balle("frizi", 1, earthObj,5, glm::vec3(1.0f, -2.0f, 0.0f),glm::vec3(-2.0f,-3.0f,0.0f));
	objectList.push_back(player1);
	player2 = new balle("planet", 1,earthObj, 5, glm::vec3(2.0f, -3.0f, 0.0f), glm::vec3(1.0f, 2.0f, 0.0f));
	objectList.push_back(player2);
	//object* floor = new object("floor", 0, floorObj);
	//objectList.push_back(floor);
	
}

void mainClass::createCamera()
{
	cameraObj = new camera(vertexObj,swapchainObj,deviceObj);
}

void mainClass::start()
{
	
	for (object* object : objectList)
	{
		CommandInfo command{};
		command.layoutPipeline = object->getModelObject()->getMaterial()->_layoutPipeline;
		command.graphicPipeline = object->getModelObject()->getMaterial()->_graphicPipepline;
		command.descriptorSets = object->getModelObject()->getDescriptorset();
		command.indexBuffer = object->getIndexBuffer();
		command.vertexBuffer = object->getVertexBuffer();
		command.indiceCount = object->getIndicesCount();
		commandInfoList.push_back(command);
	}

	commandBufferObj->commandBufferLoad(commandInfoList);
}
void mainClass::update(uint16_t imageIndex)
{	
	timeAtFrame = std::chrono::high_resolution_clock::now();
	float timePerFrame = std::chrono::duration<float, chrono::seconds::period>(timeAtFrame - LastTime).count();
	float currentTime = std::chrono::duration<float, chrono::seconds::period>(timeAtFrame - getTime()).count();
	cameraObj->update(imageIndex, 0.0f, currentTime);
	movementTime += timePerFrame; //en miliseconde
	if (movementTime > 0.01)
	{
		player1->move(player1->getVecteurDirecteur(), player1->getSpeed(), movementTime);
		player1->checkPosition();
		player2->move(player2->getVecteurDirecteur(), player2->getSpeed(), movementTime);
		player2->checkPosition();

		int distanceX = player1->getPosition().x - player2->getPosition().x;
		int distanceY = player1->getPosition().y - player2->getPosition().y;
		int norme = sqrt(pow(distanceX, 2) + pow(distanceX, 2));
		if (norme == 1)
		{
			double pourcentage = (rand() % 100)/100;
			if (player1->getMasse() < player2->getMasse())
			{
				player1->setEc(player2->getEc() * pourcentage);
				player2->setEc(-player2->getEc() * pourcentage);
			}
			else
			{
				player2->setEc(player1->getEc() * pourcentage);
				player1->setEc(-player1->getEc() * pourcentage);
			}
			int new_directionX = rand() % 4 + 1;
			int new_directionY = rand() % 4 + 1;
			glm::vec3 newDirection = glm::vec3(new_directionX, new_directionY, 0.0f);
			player1->setNewDirection(newDirection);
			newDirection = glm::vec3(-new_directionX, -new_directionY, 0.0f);
			player2->setNewDirection(newDirection);
		}
		//updateMovement(movementTime);
		movementTime = 0;
	}
	
	
	
	


	LastTime = timeAtFrame;
	string TPF = " TPF : " + to_string(timePerFrame * 1000) + "ms"; // en miliseconde
	showFPS(TPF, currentTime);	
}

void mainClass::showFPS(string ms, float currentTime)
{
	FPS++;	
	if (currentTime / nb_second >= 1)
	{
		string fps = "FPS : " + to_string(FPS) + ms;
		WindowEventObj->updateFPS(fps.c_str());
		nb_second++;

		FPS = 0;
	}
}

void mainClass::cleanup()
{
	cameraObj->cleanUp();
}
void mainClass::recreateGraphicObject()
{
	cameraObj->recreate();
	for (GraphicObject* model : modelList)
	{
		model->update();
	}
	for (object* object : objectList)
	{
		object->update();
	}
	commandInfoList.clear();
	start();
}

std::chrono::high_resolution_clock::time_point mainClass::getTime()
{
	static std::chrono::high_resolution_clock::time_point referenceTime = std::chrono::high_resolution_clock::now();
	return  referenceTime;
}

void mainClass::updateMovement(float time)
{
	keyMapping key = WindowEventObj->getMovement();
	if (key.ismove)
	{
		glm::vec3 vecteur_directeur = glm::vec3(0, 0, 0);
		if (key.movefront)
		{
			vecteur_directeur.z = 4;

		}
		if (key.moveback)
		{
			vecteur_directeur.z = -4;
		}
		if (key.moveleft)
		{
			vecteur_directeur.x = -4;

		}
		if (key.moveright)
		{
			vecteur_directeur.x = 4;

		}

		//player1->move(vecteur_directeur, glm::vec1(1), 1, time);
	}
}

mainClass::~mainClass()
{
	for (size_t i = 0; i < objectList.size(); i++)
	{
		delete objectList[i];
	}
	objectList.clear();

	for (size_t i = 0; i < modelList.size(); i++)
	{
		delete modelList[i];
	}
	modelList.clear();
}