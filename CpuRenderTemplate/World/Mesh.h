#pragma once
#include "stdafx.h"
#include "MathTypes.h"


class MeshSurface
{
public:

	int IndexCount;
	int TriangleCount;
	int VertexCount;

	glm::ivec3* Indicies;
	glm::vec3* Verticies;
	glm::vec2* UVs;

	Triangle* Triangles;


	int MaterialID;

private:
};

struct RayHit
{
	int HitIndex;
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
	std::vector<MeshSurface> Surfaces;

	void Rebuild();

	void RebuildTriangles();

	RayHit RaytraceMesh(Ray& ray, float maxDistance = FLT_MAX);


private:
};