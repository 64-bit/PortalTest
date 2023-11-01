#include "stdafx.h"
#include "RayRenderer.h"
#include "GenTexture.h"
#include <random>

using namespace glm;

RayRenderer::RayRenderer()
{
	std::random_device rd;
	std::mt19937 generator(rd()); // Mersenne Twister engine
	std::uniform_int_distribution<int> distribution; // No range specified for full range of ints

	int* colors = reinterpret_cast<int*>(_colors);
	for (int i = 0; i < RR_MAX_COLORS; i++)
	{
		colors[i] = distribution(generator);
	}
}

void RayRenderer::RenderWorld(Texture2D& target, World& world, Camera& camera)
{
	count++;

	const float fHeight = target.Height;
	const float fWidth = target.Width;

	const float fovY = tanf(camera.fovY * 0.5f);
	const float fovX = fovY * (fWidth / fHeight);

	for (int y = 0; y < target.Height; y++)
	{
		float ySSPosition = ((float)y) / fHeight;
		//ySSPosition = 1.0f - ySSPosition;
		ySSPosition = (ySSPosition * 2.0f) - 1.0f;

		float yDir = ySSPosition * fovY;

		for (int x = 0; x < target.Width; x++)
		{
			float xSSPosition = ((float)x) / fWidth;
			xSSPosition = xSSPosition;
			xSSPosition = (xSSPosition * 2.0f) - 1.0f;

			float xDir = xSSPosition * fovX;

			Ray ray = Ray(camera.Position, normalize(vec3(xDir, yDir, -1.0f)));
			ray.Direction = camera.Rotation * ray.Direction;



			int address = x + y * target.Width;
			target.Pixels[address] = ShadeRay(world, ray);
		}
	}
}

Color_b RayRenderer::ShadeRay(World& world, Ray ray)
{
	RayHit hit = world.WorldMesh.RaytraceMesh(ray);

	if (hit.HitIndex >= 0)
	{
		return _colors[hit.HitIndex % RR_MAX_COLORS];
	}

	return BackgroundColor;
}

