#include "GLFWInput.h"

GLFWInput::GLFWInput() : 
	_xpos(0), _ypos(0), _mouseButtonPressed{false, false, false} {}

void GLFWInput::updateInput(GLFWwindow* window){
	glfwPollEvents();
	glfwGetCursorPos(window, &_xpos, &_ypos);
}

bool GLFWInput::isKeyPressedOnce(int GLFW_Key, GLFWwindow* window){
	if(isKeyPressed(GLFW_Key, window)){

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

bool GLFWInput::isKeyPressed(int GLFW_Key, GLFWwindow* window) const{
	return (glfwGetKey(window, GLFW_Key) == GLFW_PRESS);
}

bool GLFWInput::isMouseButtonPressed(int GLFW_MouseButton, GLFWwindow* window) const{
	return (glfwGetMouseButton(window, GLFW_MouseButton) == GLFW_PRESS);
}

bool GLFWInput::isMouseButtonPressedOnce(int GLFW_MouseButton, GLFWwindow* window){
	if(isMouseButtonPressed(GLFW_MouseButton, window)){

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

double	GLFWInput::getXPos() const{
	return _xpos;
}

double GLFWInput::getYPos() const{
	return _ypos;
}

double GLFWInput::getXPosDiff(GLFWwindow* window) const{
	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos);

	double diff = xpos-_xpos;

	return diff;
}

double GLFWInput::getYPosDiff(GLFWwindow* window) const{
	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos);

	double diff = ypos-_ypos;

	return diff;
}

void GLFWInput::setMousePos(double xpos, double ypos, GLFWwindow* window){
	glfwSetCursorPos(window, xpos, ypos);
}
