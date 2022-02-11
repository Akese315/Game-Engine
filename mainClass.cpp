#include "mainClass.h"
mainClass::mainClass(Device* deviceObj, Vertex* vertexObj, CommandBuffer* commandBufferObj, vulkan_render* renderer, WindowEvent* WindowEventObj)
{
	this->deviceObj = deviceObj;
	this->vertexObj = vertexObj;
	this->commandBufferObj =commandBufferObj;
	this->renderer = renderer;
	this->WindowEventObj = WindowEventObj;
	FPS = 0;
	nb_second = 1;
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
	earth.pos = { 0.2, 0.3, 0.4 };

	earthObj = new GraphicObject(earth, deviceObj, vertexObj, renderer, commandBufferObj);
	modelList.push_back(earthObj);

	createObjectInfo floor{};
	floor.vertices = {
		{{-10.0f, -10.0f, -5.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{ {10.0f, -10.0f, -5.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{ {10.0f, 10.0f, -5.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		{ {-10.0f, 10.0f, -5.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};
	floor.indices = { 0, 1, 2, 2, 3, 0 };
	floor.FILE_NAME_TEXTURE = { "textures/space.jpg" };
	//floorObj = new GraphicObject(floor , deviceObj, vertexObj, renderer, commandBufferObj);
	//modelList.push_back(floorObj);
}

void mainClass::createObject()
{
	player1 = new object("frizi", 1, earthObj);
	objectList.push_back(player1);
	//object* floor = new object("floor", 0, floorObj);
	//objectList.push_back(floor);
	
}

void mainClass::createCamera()
{

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
void mainClass::update()
{
	time = std::chrono::high_resolution_clock::now();
	float timePerFrame = std::chrono::duration<float, chrono::seconds::period>(time - LastTime).count();	
	movementTime += timePerFrame; //en miliseconde
	if (movementTime > 0.001)
	{
		updateMovement(movementTime);
		movementTime = 0;
	}
	
	
	
	


	LastTime = time;
	string TPF = " TPF : " + to_string(timePerFrame * 1000) + "ms"; // en miliseconde
	showFPS(TPF);	
}

void mainClass::showFPS(string ms)
{
	FPS++;	
	float newtime = std::chrono::duration<float, chrono::seconds::period>(time - getTime()).count();
	if (newtime / nb_second >= 1)
	{
		string fps = "FPS : " + to_string(FPS) + ms;
		WindowEventObj->updateFPS(fps.c_str());
		nb_second++;

		FPS = 0;
	}
}
void mainClass::recreateGraphicObject()
{

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

		player1->move(vecteur_directeur, glm::vec1(1), 1, time);
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