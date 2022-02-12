#pragma once
#include "object.h"
#include "Type.h"
#include <math.h>
class balle:public object
{
public:
	balle(string name, uint32_t id, GraphicObject* graphicObj, int masse, glm::vec3 vecteurDirecteur, glm::vec3 posi) : object(name, id, graphicObj, posi) 
	{
		this->vecteurDirecteur = vecteurDirecteur;
		this->masse = masse;
		Ec = rand() % 10;
		getSpeedWithEc();
		setNewDirection(vecteurDirecteur);
		this->setPosition(glm::vec3(-2.0f, -3.0f, 0.0f));
		
	}
	~balle();
	glm::vec3 getVecteurDirecteur();
	float getSpeed();
	void checkPosition();
	void getSpeedWithEc();
	int getMasse();
	int getEc();
	void setEc(int newEc);
	void setNewDirection(glm::vec3 newDirection);

private:
	int masse;
	float vitesse;
	glm::vec3 vecteurDirecteur;
	float Ec;


	
	
	
}
;
