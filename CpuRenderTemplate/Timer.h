#pragma once
#include <Windows.h>

class EnigneTime
{
public:
	EnigneTime(bool printFPS, int framesToPrint);

	void OnFrame();

	int CurrentFrame;
	float DeltaTime;
	bool Print;
	int FramesToPrint;
	float AverageFrameTime;

	float TimeSinceStartup();

	double TimeSinceStartupDouble();

private:

	LARGE_INTEGER _startupTime;
	LARGE_INTEGER _lastFrameTime;
	LARGE_INTEGER _currentFrameTime;
	LARGE_INTEGER _frequency;
};

extern EnigneTime Time;

class SpanTimer
{
public:

	LARGE_INTEGER _startupTime;
	SpanTimer();

	float GetDuration();
	float GetDurationMS();


private:
	float _duration;
};