#include "stdafx.h"
#include "Timer.h"

LARGE_INTEGER PERFORMANCE_FREQUENCY;

class InitFrequency_CLASS
{
public:
    InitFrequency_CLASS() 
    {
        QueryPerformanceFrequency(&PERFORMANCE_FREQUENCY);
    }
};

InitFrequency_CLASS frequency = InitFrequency_CLASS();

SpanTimer::SpanTimer()
{
    QueryPerformanceCounter(&_startupTime);
    _duration = -1.0f;
}

float SpanTimer::GetDuration()
{
    if (_duration < 0.0f)
    {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        _duration = static_cast<double>(currentTime.QuadPart - _startupTime.QuadPart) / PERFORMANCE_FREQUENCY.QuadPart;
    }
    return _duration;
    
}

float SpanTimer::GetDurationMS()
{
    return GetDuration() * 1000.0f;
}