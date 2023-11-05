#include "stdafx.h"
#include "BVHMesh.h"
#include "Mesh.h"
#include "Timer.h"

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
			//tris.UVs[j] = source.UVs[indicies[j]];
		}

		//tris.Center = tris.ComputeCenter();
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



	_triangles = ExtractTriangles(source);

	Time time = Time(false, 0);

	BVH2::BuildBVHArgs args;
	args.MaxLeafSize = 4;
	BVH2::BVHNode<BVHTriangle>::ConstructNewBVH(_triangles, _bvh2, args);


	printf("Built BVH with %zd nodes in %f mS\n", _bvh2.size(), time.TimeSinceStartup() * 1000.0f);
}

RayHit BVHMesh::RaytraceMesh(Ray& ray, float maxDistance)
{
	FastRay fastRay = FastRay(ray);

	RayHit hit;
	hit.HitIndex = -1;

	float hitDistanceOut = maxDistance;

	//bool didHit = AttemptRayIntersection_BVH(ray, _bvh, entry, exit, hitDistanceOut, hit);
	bool didHit = AttemptRayIntersection_BVH_2(fastRay, &_bvh2[0], hitDistanceOut, hit);



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
			float thisIntersectionDistance = FLT_MAX;
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


bool BVHMesh::AttemptRayIntersection_BVH_2(const FastRay& ray, BVH2::BVHNode<BVHTriangle>* currentNode, float& outIntersectionDistance, RayHit& outHitDetails)
{
	//float oNear, oFar;
	//Try node bounds,
	//if (!currentNode->Bounds.DoesRayIntersect(ray, oNear, oFar))
	//{
	//	return false;
	//}

	bool anyValidHits = false;

	//If has children, try all children
	if (currentNode->HasChildren() == false)
	{
		BVH2::BVHNode<BVHTriangle>* nearNode = currentNode + 1;
		BVH2::BVHNode<BVHTriangle>* farNode = &_bvh2[currentNode->RightNode];

		float nearNode_entry, nearNode_exit;
		float farNode_entry, farNode_exit;

		bool nearIntersects = nearNode->Bounds.DoesRayIntersect(ray, nearNode_entry, nearNode_exit);
		bool farIntersects = farNode->Bounds.DoesRayIntersect(ray, farNode_entry, farNode_exit);

		if (nearIntersects && farIntersects)
		{
			if (farNode_entry < nearNode_entry)
			{
				//The 2 are flipped, as ordered by this ray, swap themm
				std::swap(nearNode, farNode);
				std::swap(nearNode_entry, farNode_entry);
				//std::swap(nearNode_exit, farNode_exit);
			}
		}

		if (nearIntersects && nearNode_entry < outIntersectionDistance)
		{
			if (AttemptRayIntersection_BVH_2(ray, nearNode, outIntersectionDistance, outHitDetails))
			{
				anyValidHits = true;
			}
		}

		if (farIntersects && farNode_entry < outIntersectionDistance)
		{
			if (AttemptRayIntersection_BVH_2(ray, farNode, outIntersectionDistance, outHitDetails))
			{
				anyValidHits = true;
			}
		}
	}
	else//Else, try all contents
	{

		int traceStart = currentNode->ElementStartIndex;
		int traceEnd = traceStart + currentNode->ElementCount;

		for (int i = traceStart; i < traceEnd; i++)
		{
			BVHTriangle& triangle = _triangles[i];
			float dist;

			if (triangle.RayTriangleIntersection(ray, dist))
			{
				anyValidHits = true;
				if (dist < outIntersectionDistance)
				{
					outIntersectionDistance = dist;

					outHitDetails.HitIndex = triangle.Index;
					outHitDetails.Time = dist;
				}
			}
		}
	}

	return anyValidHits;
}

RayHit BVHMesh::GetSurfaceForHit_2(const Ray& ray, float hitDistance, const BVHTriangle& hitTriangle)
{
	RayHit hit;
	hit.HitIndex = hitTriangle.Index;
	hit.Time = hitDistance;
	return hit;
}

BVHTriangle BVHMesh::GetTriangle(int index) const
{
	return _triangles[index];
}