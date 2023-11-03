#include "stdafx.h"
#include "BVHMesh.h"
#include "Mesh.h"

using namespace std;
using namespace glm;


BVHMesh::BVHMesh()
{

}

vector<BVHTriangle> ExtractTriangles(Mesh& source)
{
	vector<BVHTriangle> result;

	for (int i = 0; i < source.TriangleCount; i++)
	{
		ivec3 indicies = source.Indicies[i];

		BVHTriangle tris;
		tris.Index = i;

		tris.A = source.Verticies[indicies.x];
		tris.B = source.Verticies[indicies.y];
		tris.C = source.Verticies[indicies.z];

		for (int j = 0; j < 3; j++)
		{
			tris.UVs[j] = source.UVs[indicies[j]];
		}

		tris.Center = tris.ComputeCenter();
		result.push_back(tris);
	}

	return result;
}

BVHMesh::BVHMesh(Mesh& source)
{
	//Do this here, trace and it's done
	vector<BVHTriangle> sourceTriangles = ExtractTriangles(source);
	_bvh = BVHNode<BVHTriangle>::BuildBVH(sourceTriangles, 4);
	_boundingBoxWorldspace = _bvh->Bounds;
}

RayHit BVHMesh::RaytraceMesh(Ray& ray, float maxDistance)
{

	RayHit hit;
	hit.HitIndex = -1;

	float entry = 0.0f;
	float exit = FLT_MAX;
	float hitDistanceOut = maxDistance;

	bool didHit = AttemptRayIntersection_BVH(ray, _bvh, entry, exit, hitDistanceOut, hit);

	return hit;
}


bool BVHMesh::AttemptRayIntersection_BVH(const Ray& ray, BVHNode<BVHTriangle>* currentNode, float entry, float exit, float& outIntersectionDistance, RayHit& outHitDetails)
{
	float oNear, oFar;
	//Try node bounds,
	if (!currentNode->Bounds.DoesRayIntersect(ray, oNear, oFar))
	{
		return false;
	}

	bool anyValidHits = false;

	//If has children, try all children
	if (currentNode->ChildNodes.size() > 0)
	{
		for (auto& child : currentNode->ChildNodes)
		{
			float thisIntersectionDistance = std::numeric_limits<float>::max();
			RayHit thisHitSurface;
			if (AttemptRayIntersection_BVH(ray, child, entry, exit, thisIntersectionDistance, thisHitSurface))
			{
				anyValidHits = true;
				if (thisIntersectionDistance < outIntersectionDistance)
				{
					outIntersectionDistance = thisIntersectionDistance;
					outHitDetails = thisHitSurface;
				}
			}
		}
	}
	else//Else, try all contents
	{
		for (auto& triangle : currentNode->ContentsVector)
		{
			float dist;

			if (triangle.RayTriangleIntersection(ray, dist))
			{
				anyValidHits = true;
				if (dist < outIntersectionDistance)
				{
					outIntersectionDistance = dist;
					outHitDetails = GetSurfaceForHit(ray, dist, triangle);
				}
			}
		}
	}

	return anyValidHits;
}

RayHit BVHMesh::GetSurfaceForHit(const Ray& ray, float hitDistance, const BVHTriangle& hitTriangle)
{
	RayHit hit;
	hit.HitIndex = hitTriangle.Index;
	hit.Time = hitDistance;
	return hit;
}
