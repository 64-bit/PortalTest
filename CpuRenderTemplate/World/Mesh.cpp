#include "stdafx.h"
#include "Mesh.h"

using namespace std;

Mesh::Mesh()
{
	IndexCount = 0;
	Verticies = 0;
	UVs = 0;
	TriangleCount = 0;

	Indicies = nullptr;
	Verticies = nullptr;
	UVs = nullptr;

	Triangles = nullptr;
}

void Mesh::Rebuild()
{
	RebuildTriangles();
	RebuildToSurface();
}

void Mesh::RebuildTriangles()
{
	if (Triangles != nullptr)
	{
		delete[] Triangles;
	}

	Triangles = new Triangle[TriangleCount];

	for (int i = 0; i < TriangleCount; i++)
	{
		Triangles[i] = Triangle::MakeTriangle(Indicies, Verticies, i);
	}
}

void Mesh::RebuildToSurface()
{
	TriangleToSurfaceIndex = vector<int>(TriangleCount, -1);
	for (int i = 0; i < Surfaces.size(); i++)
	{
		MeshSurface currentSurface = Surfaces[i];
		int end = currentSurface.IndexStart/3 + currentSurface.IndexCount/3;
		for (int j = currentSurface.IndexStart/3; j < end; j++)
		{
			TriangleToSurfaceIndex[j] = i;
		}
	}
}

RayHit Mesh::RaytraceMesh(Ray& ray, float maxDistance)
{
	RayHit result;
	result.HitIndex = -1;

	for (int i = 0; i < TriangleCount; i++)
	{
		Triangle triangle = Triangles[i];
		float hitDistance;

		if (triangle.RayTriangleIntersection(ray, hitDistance))
		{
			if (hitDistance < maxDistance)
			{
				maxDistance = hitDistance;
				result.HitIndex = i;
			}
		}
	}

	result.Time = maxDistance;
	return result;
}

