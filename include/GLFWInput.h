#ifndef _GLFWINPUT_
#define _GLFWINPUT_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GLFW\glfw3.h>
#endif

#include <map>
#include <algorithm>

class GLFWInput{
private:
	mutable double	_xpos, _ypos;
	std::map<int, int>	_keyPressed;
	bool	_mouseButtonPressed[3];

public:
	GLFWInput();

	void updateInput(GLFWwindow*);

	bool isKeyPressed(int GLFW_Key, GLFWwindow*) const;
	bool isMouseButtonPressed(int GLFW_MouseButton, GLFWwindow*) const;
	
	double getXPos() const;
	double getYPos() const;
	
	double getXPosDiff(GLFWwindow*) const;
	double getYPosDiff(GLFWwindow*) const;
	
	bool isKeyPressedOnce(int GLFW_Key, GLFWwindow* window);
	bool isMouseButtonPressedOnce(int GLFW_MouseButton, GLFWwindow* window);
	
	void setMousePos(double xpos, double ypos, GLFWwindow*);
};


#endif