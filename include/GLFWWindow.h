#ifndef _GLFWWINDOW_
#define _GLFWWINDOW_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GLFW\glfw3.h>
#endif

#include <stdlib.h>
#include <stdexcept>


class GLFWWindow{
private:
	int					_width, _height;
	int					_windowHandle;
	std::string	_windowName;
	bool				_windowed;
	GLFWwindow*	_window;
	
public: 
	GLFWWindow(int width, int height, char* windowName, bool windowed){
		_width = width;
		_height = height;

		_windowName = std::string(windowName);

		_windowed = windowed;

		initialize();
		setVSync(false);
	}

	~GLFWWindow(){
		glfwTerminate();
	}

	int getWidth(){
		return _width;
	}

	int getHeight(){
		return _height;
	}
	
	GLFWwindow* getGLFWwindow(){
		return _window;
	}
	
	void swapBuffers(){
		glfwSwapBuffers(_window);
	}
	
	void setWindowTitle(const char *title){
		glfwSetWindowTitle(_window, title);
	}

	void setDefaultWindowTitle(){
		glfwSetWindowTitle(_window, _windowName.c_str());
	}

	void setVSync(bool enable){
		glfwSwapInterval( enable?1:0);
	}


private:

	void initialize(){	
		if(glfwInit() != GL_TRUE){
			throw std::runtime_error("Could not initialize GLFW!");
		}
		
		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		_window = glfwCreateWindow(_width, _height, _windowName.c_str(), _windowed?NULL:glfwGetPrimaryMonitor(), NULL);
		
		if(!_window){
			glfwTerminate();
			throw std::runtime_error("Could not open GLFW Window!");
		} 

		setWindowTitle(_windowName.c_str());
		
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwMakeContextCurrent(_window);
	}
	
};


#endif