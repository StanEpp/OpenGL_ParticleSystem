#ifndef _GLFWWINDOW_
#define _GLFWWINDOW_

#ifndef _GLEW_
#define _GLEW_
  #include <GL\gl3w.h>
  #include <GLFW\glfw3.h>
#endif

#include <stdexcept>

class GLFWWindow{
private:
  int _width, _height;
  std::string _windowName;
  bool  _fullscreen, _vSync, _initialized;
  GLFWwindow * _window;
  
public: 
  GLFWWindow() : _width(1024), _height(768), _windowName("GLFW Window"), _fullscreen(false), _vSync(false), _initialized(false), _window(nullptr) {} 
  GLFWWindow(int width, int height, const std::string& windowName, bool fullscreen, bool vSync = false) : 
    _width(width), _height(height), _windowName(windowName), _fullscreen(fullscreen), _vSync(vSync), _initialized(false), _window(nullptr) {}
  GLFWWindow(GLFWWindow&) = delete;
  GLFWWindow(GLFWWindow&&) = delete;
  GLFWWindow& operator=(GLFWWindow&) = delete;
  GLFWWindow& operator=(GLFWWindow&&) = delete;

  ~GLFWWindow(){
    if(!_initialized) return;
    glfwMakeContextCurrent(nullptr);
    if(_window) glfwDestroyWindow(_window);
    glfwTerminate();
  }

  int getWidth() const{
    return _width;
  }

  int getHeight() const{
    return _height;
  }
  
  GLFWwindow* getGLFWwindow() const{
    return _window;
  }
  
  void swapBuffers(){
    glfwSwapBuffers(_window);
  }
  
  void setWindowTitle(const std::string& title){
    glfwSetWindowTitle(_window, title.c_str());
  }

  void setDefaultWindowTitle(){
    glfwSetWindowTitle(_window, _windowName.c_str());
  }

  void setVSync(bool enable){
    _vSync = enable;
    glfwSwapInterval(_vSync?0:1);
  }
  
  bool isVSyncOn(){
    return _vSync;
  }

  void initialize(){
    if(!glfwInit()){
      throw std::runtime_error("Could not initialize GLFW!");
    }
    
    _initialized = true;
    
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    _window = glfwCreateWindow(_width, _height, _windowName.c_str(), _fullscreen?glfwGetPrimaryMonitor():0, 0);
    
    if(!_window){
      throw std::runtime_error("Could not open GLFW Window!");
    } 

    setDefaultWindowTitle();
    
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(_window);
    
    glfwSwapInterval(_vSync?0:1);
  }
};


#endif