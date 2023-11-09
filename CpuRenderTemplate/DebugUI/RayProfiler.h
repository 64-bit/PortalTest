#pragma once

class RayProfiler_C
{
public:

	void ResetFrame();

	inline int GetRayCount();

	inline void AddRays(int rays);

private:
	int _rays;
};

inline int RayProfiler_C::GetRayCount()
{
	return _rays;
}

inline void RayProfiler_C::AddRays(int rays)
{
	_rays += rays;
}

extern RayProfiler_C RayProfiler;