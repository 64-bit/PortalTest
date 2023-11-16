#include "stdafx.h"
#include "Timer.h"
#include <iostream>

EnigneTime Time = EnigneTime(true, 100);

EnigneTime::EnigneTime(bool printFPS, int framesToPrint)
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

void EnigneTime::OnFrame()
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

float EnigneTime::TimeSinceStartup()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	return static_cast<double>(currentTime.QuadPart - _startupTime.QuadPart) / _frequency.QuadPart;
}

double EnigneTime::TimeSinceStartupDouble()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	return static_cast<double>(currentTime.QuadPart - _startupTime.QuadPart) / _frequency.QuadPart;
}