#include "balle.h"


void balle::checkPosition()
{
	if (this->getPosition().x > 5 || this->getPosition().x <= -5)
	{
		vecteurDirecteur.x *= -1;
	}
	if (this->getPosition().y > 5 || this->getPosition().y <= -5)
	{
		vecteurDirecteur.y *= -1;
	}
}

void balle::getSpeedWithEc()
{
	vitesse = sqrt(Ec / ((float)masse * 1.0f / 2.0f))+1.0f;	
}

int balle::getMasse()
{
	return masse;
}

glm::vec3 balle::getVecteurDirecteur()
{
	return vecteurDirecteur;
}

void balle::setNewDirection(glm::vec3 setNewDirection)
{	
	vecteurDirecteur.x = setNewDirection.x*vitesse;
	vecteurDirecteur.y = setNewDirection.y*vitesse;
}

float balle::getSpeed()
{
	return vitesse;
}

int balle::getEc()
{
	return Ec;
}

void balle::setEc(int newEc)
{
	Ec += newEc;
}

balle::~balle()
{

}