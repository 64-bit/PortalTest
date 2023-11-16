#include "stdafx.h"
#include "Profiler.h"

using namespace std;

Profiler FrameProfiler = Profiler();

Profiler::Profiler()
{
    _lastFrame = std::make_shared<ProfilerFrame>();
    _currentFrame = std::make_shared<ProfilerFrame>();
}

void Profiler::StartFrame()
{
    //End current frame
    _currentFrame->TimeSpan.GetDuration();

    _lastFrame = _currentFrame;
    _currentFrame = std::make_shared<ProfilerFrame>();
}

void Profiler::EndFrame()
{

}

void Profiler::PushSection(const std::string& sectionName)
{
    _currentFrame->Sections.push_back(ProfilerSection(sectionName));
}

void Profiler::PopSection()
{
    if (_currentFrame->Sections.size() > 0)
    {
        _currentFrame->Sections.back().SectionSpan.GetDuration();
    }
}

shared_ptr<ProfilerFrame> Profiler::GetLastFrameData()
{
    return _lastFrame;
}

ProfilerSection::ProfilerSection(const std::string& name)
    : SectionName(name)
{
    SectionSpan = SpanTimer();
}

