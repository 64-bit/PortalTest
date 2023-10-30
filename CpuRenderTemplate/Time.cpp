#include "Time.h"
#include <iostream>


Time::Time(bool printFPS, int framesToPrint)
{
	Print = printFPS;
	FramesToPrint = framesToPrint;

	QueryPerformanceCounter(&_startupTime);
	_currentFrameTime = _startupTime;
	_lastFrameTime = _startupTime;

	QueryPerformanceFrequency(&_frequency);

	AverageFrameTime = 0.0f;
	CurrentFrame = 0;
	DeltaTime = 0.0f;
}

void Time::OnFrame()
{
	CurrentFrame++;

	QueryPerformanceCounter(&_currentFrameTime);

	double frameTime =  static_cast<double>(_currentFrameTime.QuadPart - _lastFrameTime.QuadPart) / _frequency.QuadPart;
	AverageFrameTime = AverageFrameTime * 0.9f + frameTime * 0.1f;
	DeltaTime = frameTime;

	_lastFrameTime = _currentFrameTime;

	if (Print && CurrentFrame % FramesToPrint == 0)
	{
		printf("Frame %d Time mS: %f\n", CurrentFrame, AverageFrameTime * 1000.0f);
	}
}

float Time::TimeSinceStartup()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	return static_cast<double>(_currentFrameTime.QuadPart - _startupTime.QuadPart) / _frequency.QuadPart;
}
