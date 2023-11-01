#pragma once
#include <Windows.h>

class Time
{
public:
	Time(bool printFPS, int framesToPrint);

	void OnFrame();

	int CurrentFrame;
	float DeltaTime;
	bool Print;
	int FramesToPrint;
	float AverageFrameTime;

	float TimeSinceStartup();

private:

	LARGE_INTEGER _startupTime;
	LARGE_INTEGER _lastFrameTime;
	LARGE_INTEGER _currentFrameTime;
	LARGE_INTEGER _frequency;
};