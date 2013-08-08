#ifndef _TIMER_
#define _TIMER_

class Timer{
private:
	double		_countsPerSecond;
	__int64		_counterStart;
	int			_frameCount;
	int			_fps;
	__int64		_frameTimeOld;
	double		_frameTime;


public:
	Timer();
	~Timer();

	void startTimer();
	double getTime();
	double getFrameTime();
	double getFrameTimeWithoutActualisation();
};

#endif