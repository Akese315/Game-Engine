#include "WindowEvent.h"


WindowEvent::WindowEvent(GLFWwindow* window)
{
	this->window = window;
}
keyMapping WindowEvent::getMovement()
{
	keyMapping keyPressed;
	if (IsMovingFront())
	{
		keyPressed.movefront = true;
		keyPressed.ismove = true;
	}
	
	if (IsMovingBack())
	{
		keyPressed.moveback = true;
		keyPressed.ismove = true;
	}
	
	if (IsMovingRight())
	{
		keyPressed.moveright = true;
		keyPressed.ismove = true;
	}
	
	if (IsMovingLeft())
	{
		keyPressed.moveleft = true;
		keyPressed.ismove = true;
	}
	
	return keyPressed;
}

bool WindowEvent::IsMovingFront()
{
	if (glfwGetKey(window, movefront) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}

bool WindowEvent::IsMovingBack()
{
	if (glfwGetKey(window, moveback) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}

bool WindowEvent::IsMovingRight()
{
	if (glfwGetKey(window, moveright) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}


bool WindowEvent::IsMovingLeft()
{
	if (glfwGetKey(window, moveleft) == GLFW_PRESS)
	{
		return true;
	}
	
	return false;
}

void WindowEvent::updateFPS(const char* title)
{
	glfwSetWindowTitle(window, title);
}