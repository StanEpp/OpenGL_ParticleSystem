#ifndef _GLFWTIMER_
#define _GLFWTIMER_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GL\glfw.h>
#endif

class GLFWTimer{
private:
	double		_time;
	double		_oldTime;

public:
	GLFWTimer() : _time(0), _oldTime(0){}
	~GLFWTimer(){}

	//Receives time since glfwInit() call
	double	getTime(){
		return _time + glfwGetTime();
	}	

	//Returns the time difference since the last getTimeDiff() call
	double	getTimeDiff(){
		double currTime = getTime();
		double timeDiff = currTime - _oldTime;
		_oldTime = currTime;
		
		return timeDiff;
	}

	//Returns the time difference since the last getTimeDiff() call
	double getTimeDiffWithoutActualization(){
		double currTime = getTime();
		double timeDiff = currTime - _oldTime;
		
		return timeDiff;
	}

	//Returns the time since the last resetTime() call
	double	getRefreshedTime(){
		return glfwGetTime();
	}

	//Resets the time to 0
	void	resetTime(){
		_time += glfwGetTime();
		glfwSetTime(0.0);
	}
};

#endif