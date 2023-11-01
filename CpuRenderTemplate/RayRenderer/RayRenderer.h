#pragma once

#include "World/World.h"
#include "Graphics/Texture2D.h"
#include "World/Camera.h"

#define RR_MAX_COLORS 256

class RayRenderer
{
public:

	RayRenderer();

	void RenderWorld(Texture2D& target, World& world, Camera& camera);

	Color_b BackgroundColor;

private:


	int count;


	Color_b ShadeRay(World& world, Ray ray);
	Color_b _colors[RR_MAX_COLORS];
};