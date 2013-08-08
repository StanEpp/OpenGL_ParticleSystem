#ifndef _GLFWWINDOW_
#define _GLFWWINDOW_

#include <stdlib.h>
#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GL\glfw.h>
#endif

class GLFWWindow{
private:
	int			_width, _height;
	int			_windowHandle;
	std::string	_windowName;
	bool		_windowed;

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

	void swapBuffers(){
		glfwSwapBuffers();
	}
	
	void setWindowTitle(const char *title){
		glfwSetWindowTitle(title);
	}

	void setDefaultWindowTitle(){
		glfwSetWindowTitle(_windowName.c_str());
	}

	void setVSync(bool enable){
		glfwSwapInterval( enable?1:0);
	}


private:
	void initialize(){
		
		if(glfwInit() != GL_TRUE){
			throw std::exception("Could not initialize GLFW!");
		}

		glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
		glfwOpenWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if(glfwOpenWindow(_width, _height, 8, 8, 8, 8, 16, 8, _windowed?GLFW_WINDOW:GLFW_FULLSCREEN) != GL_TRUE){
			glfwTerminate();
			throw std::exception("Could not open GLFW Window!");
		} 

		setWindowTitle(_windowName.c_str());

		if(!_windowed){
			glfwEnable(GLFW_MOUSE_CURSOR);
		}

	}

	
};


#endif _GLFWWINDOW_