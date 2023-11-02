#include "stdafx.h"
#include "Mesh.h"

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

	return result;
}

