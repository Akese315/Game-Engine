#pragma once
#include <GLFW/glfw3.h>
#include "type.h"
class WindowEvent
{
public :
	
	const int movefront = GLFW_KEY_W;
	const int moveback = GLFW_KEY_S;
	const int moveleft = GLFW_KEY_A;
	const int moveright = GLFW_KEY_D;
	
	keyMapping getMovement();
	bool IsMovingFront();
	bool IsMovingBack();
	bool IsMovingRight();
	bool IsMovingLeft();
	void updateFPS(const char* title);
	WindowEvent(GLFWwindow * window);

private:
	GLFWwindow* window;
};

