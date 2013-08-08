#include "GLFWInput.h"
#include <stdlib.h>

GLFWInput::GLFWInput() : _xpos(0), _ypos(0){
	_mouseButtonPressed[0] = _mouseButtonPressed[1] = _mouseButtonPressed[2] = false;
}

GLFWInput::~GLFWInput(){

}

void GLFWInput::updateInput(){
	glfwPollEvents();
	glfwGetMousePos(&_xpos, &_ypos);

}

bool GLFWInput::isKeyPressedOnce(int GLFW_Key){
	if(isKeyPressed(GLFW_Key)){

		if(_keyPressed.find(GLFW_Key) == _keyPressed.end()){
				_keyPressed.insert(std::pair<int, int>(GLFW_Key, GLFW_Key));
				return true;
		} else {
			return false;
		}

	} else {
		_keyPressed.erase(GLFW_Key);
		return false;
	}
}

bool GLFWInput::isKeyPressed(int GLFW_Key){
	return (glfwGetKey(GLFW_Key) == GLFW_PRESS);
}

bool GLFWInput::isMouseButtonPressed(int GLFW_MouseButton){
	return (glfwGetMouseButton(GLFW_MouseButton) == GLFW_PRESS);
}

bool GLFWInput::isMouseButtonPressedOnce(int GLFW_MouseButton){
	if(isMouseButtonPressed(GLFW_MouseButton)){

		if(!_mouseButtonPressed[GLFW_MouseButton]){
				_mouseButtonPressed[GLFW_MouseButton] = true;
				return true;
		} else {
			return false;
		}

	} else {
		_mouseButtonPressed[GLFW_MouseButton] = false;
		return false;
	}
}

int	GLFWInput::getXPos(){
	return _xpos;
}

int GLFWInput::getYPos(){
	return _ypos;
}

int GLFWInput::getXPosDiff(){
	int xpos, ypos;

	glfwGetMousePos(&xpos, &ypos);

	int diff = xpos-_xpos;

	return diff;
}

int GLFWInput::getYPosDiff(){
	int xpos, ypos;

	glfwGetMousePos(&xpos, &ypos);

	int diff = ypos-_ypos;

	return diff;
}

int GLFWInput::getMouseWheel(){
	return glfwGetMouseWheel();
}

void GLFWInput::setMousePos(int xpos, int ypos){
	glfwSetMousePos(xpos, ypos);
}

void GLFWInput::setMouseWheel(int pos){
	glfwSetMouseWheel(pos);
}