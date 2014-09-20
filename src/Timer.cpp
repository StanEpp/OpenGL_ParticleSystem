#include "Timer.h"
#include <Windows.h>

Timer::Timer() : _countsPerSecond(0), _counterStart(0), _frameCount(0),
							   _fps(0), _frameTimeOld(0), _frameTime(0){

}

Timer::~Timer(){

}

void Timer::startTimer(){
	LARGE_INTEGER	frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	_countsPerSecond = static_cast<double>(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	_counterStart = frequencyCount.QuadPart;
}

double Timer::getTime(){
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	
	return( static_cast<double>(currentTime.QuadPart - _counterStart)/_countsPerSecond );
}

double Timer::getFrameTime(){
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - _frameTimeOld;
	_frameTimeOld = currentTime.QuadPart;

	if(tickCount < 0.0f){
		tickCount = 0;
	}

	return static_cast<double>(tickCount/_countsPerSecond);
}

double Timer::getFrameTimeWithoutActualisation(){
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - _frameTimeOld;

	if(tickCount < 0.0f){
		tickCount = 0;
	}

	return static_cast<double>(tickCount/_countsPerSecond);
}
