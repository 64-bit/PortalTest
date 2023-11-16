#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include <vector>

#include "Timer.h"

class ProfilerSection
{
public:

public:

	ProfilerSection(const std::string& name);

	const std::string SectionName;
	SpanTimer SectionSpan;
};

class ProfilerFrame
{
public:

	SpanTimer TimeSpan;
	std::vector< ProfilerSection> Sections;
};


class Profiler
{
public:
	Profiler();

	void StartFrame();

	void EndFrame();

	void PushSection(const std::string& sectionName);

	void PopSection();

	std::shared_ptr<ProfilerFrame> GetLastFrameData();

private:
	std::shared_ptr<ProfilerFrame> _lastFrame;
	std::shared_ptr<ProfilerFrame> _currentFrame;
};

extern Profiler FrameProfiler;