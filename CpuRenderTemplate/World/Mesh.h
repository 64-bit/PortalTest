#pragma once
#include "stdafx.h"
#include "MathTypes.h"

class Texture2D;

class MeshSurface
{
public:

	Texture2D* Texture;
	int IndexStart;
	int IndexCount;

private:
};

struct RayHit
{
	int HitIndex;
	float Time;

	inline bool DidHit() {
		return HitIndex >= 0;
	}
};

class Mesh
{
public:
	Mesh();

	int IndexCount;
	int TriangleCount;
	int VertexCount;

	glm::ivec3 * Indicies;
	glm::vec3* Verticies;
	glm::vec2* UVs;

	Triangle* Triangles;
	std::vector<int> TriangleToSurfaceIndex;
	std::vector<MeshSurface> Surfaces;

	void Rebuild();

	void RebuildTriangles();
	void RebuildToSurface();

	RayHit RaytraceMesh(Ray& ray, float maxDistance = FLT_MAX);


private:
};