#ifndef _GLFWINPUT_
#define _GLFWINPUT_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GL\glfw.h>
#endif

#include <map>
#include <algorithm>
#include "Input.h"

class GLFWInput : public Input{
private:
	int					_xpos, _ypos;
	std::map<int, int>	_keyPressed;
	bool				_mouseButtonPressed[3];

public:
	GLFWInput();
	~GLFWInput();

	void updateInput();

	bool isKeyPressed(int GLFW_Key);
	bool isMouseButtonPressed(int GLFW_MouseButton);
	int	getXPos();
	int getYPos();
	int getMouseWheel();

	int getXPosDiff();
	int getYPosDiff();

	bool isKeyPressedOnce(int GLFW_Key);
	bool isMouseButtonPressedOnce(int GLFW_MouseButton);

	void setMousePos(int xpos, int ypos);
	void setMouseWheel(int pos);
};


#endif