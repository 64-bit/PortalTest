#include "stdafx.h"
#include "RayRenderer.h"
#include "GenTexture.h"
#include <random>
#include "World/Mesh.h"
#include "DebugUI/RayProfiler.h"

using namespace glm;

#define MAX_DEPTH 30

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
			target.Pixels[address] = ShadeRay(world, ray, 0);
		}
	}
}

Color_b RayRenderer::ShadeRay(World& world, Ray ray, int depth)
{
	RayProfiler.AddRays(1);

	//RayHit hit = world.WorldMesh.RaytraceMesh(ray);
	RayHit hit = world.WorldBVHMesh.RaytraceMesh(ray);

	if (hit.HitIndex >= 0)
	{
		//Check and see if this is a portal
		int portalIndex = world.PortalIndicies[hit.HitIndex];
		if (portalIndex >= 0)
		{
			if (depth > MAX_DEPTH)
			{
				return Color_b(255, 0, 0, 255);
			}

			if (portalIndex < 0 || portalIndex >= world.WorldMesh.TriangleCount)
			{
				printf("Oh shit: %d", portalIndex);
			}

			//Transform ray by portal
			mat4 portalTransform = world.Portals[portalIndex];
			//portalTransform = transpose(portalTransform);

			Ray newRay = ray;
			newRay.Origin = ray.Along(hit.Time + 0.0001f);
			newRay.Direction = ray.Direction;

			newRay = newRay.Transform(portalTransform);

			//Recurse
			return ShadeRay(world, newRay, depth + 1);
		}

		//Shade hit, no portal was taken
		return ShadeFragmment(world, ray, hit);


	}

	return BackgroundColor;
}

Color_b RayRenderer::ShadeFragmment(World& world, Ray ray, RayHit hit)
{
	vec3 verts[3];
	vec2 uvs[3];

	//int remappedHit = world.WorldBVHMesh.GetTriangle(hit.HitIndex).Index;

	ivec3 indices = world.WorldMesh.Indicies[hit.HitIndex];

	for (int i = 0; i < 3; i++)
	{
		verts[i] = world.WorldMesh.Verticies[indices[i]];
		uvs[i] = world.WorldMesh.UVs[indices[i]];
	}

	vec3 hitPos = ray.Along(hit.Time);
	vec3 byCords = Triangle::ComputeByCords(verts[0], verts[1], verts[2], hitPos);

	vec2 finalUV = vec2(0.0f);

	for (int i = 0; i < 3; i++)
	{
		finalUV += uvs[i] * byCords[i];
	}

	finalUV *= 0.25f;
	finalUV = abs(finalUV);
	//Color_b color = _colors[hit.HitIndex % RR_MAX_COLORS];;
	//color.r = finalUV.x * 255.0f;
	//color.g = finalUV.y * 255.0f;

	int surfaceIndex = world.WorldMesh.TriangleToSurfaceIndex[hit.HitIndex];
	if (surfaceIndex >= 0)
	{
		const MeshSurface& surface = world.WorldMesh.Surfaces[surfaceIndex];
		if (surface.Texture == nullptr)
		{
			return _colors[hit.HitIndex % 255];
		}
		else
		{
			return surface.Texture->Sample(finalUV);
		}
	}
	else
	{
		return Color_b(0, 255, 32, 255);
	}


	//return color;
}

