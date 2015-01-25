#ifndef _GLFWTIMER_
#define _GLFWTIMER_

#include <GLFW\glfw3.h>

class GLFWTimer{
private:
	double		_time;
	mutable double	_oldTime;

public:
	GLFWTimer() : _time(0), _oldTime(0){}

	//Receives time since glfwInit() call
	double	getTime() const{
		return _time + glfwGetTime();
	}

	//Returns the time difference since the last getTimeDiff() call
	double	getTimeDiff() const{
		double currTime = getTime();
		double timeDiff = currTime - _oldTime;
		_oldTime = currTime;
		
		return timeDiff;
	}

	//Returns the time difference since the last getTimeDiff() call
	double getTimeDiffWithoutActualization() const{
		double currTime = getTime();
		double timeDiff = currTime - _oldTime;
		
		return timeDiff;
	}

	//Returns the time since the last resetTime() call
	double	getRefreshedTime() const{
		return glfwGetTime();
	}

	//Resets the time to 0
	void	resetTime(){
		_time += glfwGetTime();
		glfwSetTime(0.0);
	}
};

#endif