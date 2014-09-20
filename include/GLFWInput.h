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
	double							_xpos, _ypos;							
	std::map<int, int>	_keyPressed;
	bool								_mouseButtonPressed[3];

public:
	GLFWInput();
	~GLFWInput();

	void updateInput(GLFWwindow*);

	bool isKeyPressed(int GLFW_Key, GLFWwindow*);
	
	bool isMouseButtonPressed(int GLFW_MouseButton, GLFWwindow*);
	
	double getXPos();
	double getYPos();
	
	double getXPosDiff(GLFWwindow*);
	double getYPosDiff(GLFWwindow*);
	
	bool isKeyPressedOnce(int GLFW_Key, GLFWwindow* window);
	bool isMouseButtonPressedOnce(int GLFW_MouseButton, GLFWwindow* window);
	
	void setMousePos(double xpos, double ypos, GLFWwindow*);
};


#endif