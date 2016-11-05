#ifndef _GLFWINPUT_
#define _GLFWINPUT_

#ifndef _GL3W_
#define _GL3W_
  #include <GL\gl3w.h>
  #include <GLFW\glfw3.h>
#endif

#include <unordered_set>
#include <array>
#include "GLFWWindow.h"

class GLFWInput{
private:
  double              _xPos, _yPos;
  double              _xDiff, _yDiff;
  std::array<bool, 3> _mouseButtonPressed;
  std::unordered_set<unsigned int>  _keyPressed;
  GLFWwindow* _window;

public:
  GLFWInput() : _xPos(0), _yPos(0), _xDiff(0), _yDiff(0), _mouseButtonPressed{ false, false, false }, _window(nullptr) {}
  
  void bindInputToWindow(const GLFWWindow& window){
    _window = window.getGLFWwindow();
  }
  
  void updateInput() {
    double x, y;
    glfwPollEvents();
    glfwGetCursorPos(_window, &x, &y);
    _xDiff = x - _xPos;
    _yDiff = y - _yPos;
    _xPos = x;
    _yPos = y;
  }

  int isKeyPressed(unsigned int GLFW_Key) const {
    return (glfwGetKey(_window, GLFW_Key) == GLFW_PRESS);
  }

  bool isMouseButtonPressed(unsigned int GLFW_MouseButton) const{
    return (glfwGetMouseButton(_window, GLFW_MouseButton) == GLFW_PRESS);
  }

  double getXPos() const {
    return _xPos;
  }
  double getYPos() const {
    return _yPos;
  }

  double getXPosDiff() const {
    return _xDiff;
  }
  double getYPosDiff() const {
    return _yDiff;
  }

  bool isKeyPressedOnce(unsigned int GLFW_Key){
    if (isKeyPressed(GLFW_Key)){
      if (_keyPressed.find(GLFW_Key) == _keyPressed.end()){
        _keyPressed.insert(GLFW_Key);
        return true;
      }
    }
    else {
      _keyPressed.erase(GLFW_Key);
    }
    return false;
  }

  bool isMouseButtonPressedOnce(unsigned int GLFW_MouseButton){
    if (isMouseButtonPressed(GLFW_MouseButton)){
      if (!_mouseButtonPressed[GLFW_MouseButton]){
        _mouseButtonPressed[GLFW_MouseButton] = true;
        return true;
      }
    }
    else {
      _mouseButtonPressed[GLFW_MouseButton] = false;
    }
    return false;
  }

  void setMousePos(double xpos, double ypos){
    glfwSetCursorPos(_window, xpos, ypos);
  }

};

#endif