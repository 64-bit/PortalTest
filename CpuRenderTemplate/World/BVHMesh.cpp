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

	EnigneTime time = EnigneTime(false, 0);

	BVH2::BuildBVHArgs args;
	args.MaxLeafSize = 5;
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
	AttemptRayIntersection_BVH_2(fastRay, &_bvh2[0], hitDistanceOut, hit);
	//AttemptRayIntersection_BVH_2_While(fastRay, &_bvh2[0], hitDistanceOut, hit);


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


void BVHMesh::AttemptRayIntersection_BVH_2(const FastRay& ray, BVH2::BVHNode<BVHTriangle>* currentNode, float& outIntersectionDistance, RayHit& outHitDetails)
{
	//float oNear, oFar;
	//Try node bounds,
	//if (!currentNode->Bounds.DoesRayIntersect(ray, oNear, oFar))
	//{
	//	return false;
	//}

	//bool anyValidHits = false;

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
			AttemptRayIntersection_BVH_2(ray, nearNode, outIntersectionDistance, outHitDetails);
		}

		if (farIntersects && farNode_entry < outIntersectionDistance)
		{
			AttemptRayIntersection_BVH_2(ray, farNode, outIntersectionDistance, outHitDetails);
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
				//anyValidHits = true;
				if (dist < outIntersectionDistance)
				{
					outIntersectionDistance = dist;

					outHitDetails.HitIndex = triangle.Index;
					outHitDetails.Time = dist;
				}
			}
		}
	}

	//return anyValidHits;
}

struct LoopState
{
	BVH2::BVHNode<BVHTriangle>* NodeTarget;
	float NearHitTime;

	inline LoopState();

	inline LoopState(BVH2::BVHNode<BVHTriangle>* target, float nearHitTIme);
};

inline LoopState::LoopState()
{

}

inline LoopState::LoopState(BVH2::BVHNode<BVHTriangle>* target, float nearHitTIme)
{
	NodeTarget = target;
	NearHitTime = nearHitTIme;
}

struct LocalStack
{
	int stackPointer = -1;
	LoopState stack[32];
	LoopState* RealStackPointer;

	inline LoopState Pop();
	
	inline void Push(const LoopState& state);

	inline bool HasItems();
};

inline LoopState LocalStack::Pop() 
{
	LoopState retVal = *RealStackPointer;
	RealStackPointer--;
	return retVal;
}

inline void LocalStack::Push(const LoopState& state)
{
	RealStackPointer++;
	*RealStackPointer = state;
}

inline bool LocalStack::HasItems()
{
	return RealStackPointer >= &stack[0];
}

void BVHMesh::AttemptRayIntersection_BVH_2_While(const FastRay& ray, BVH2::BVHNode<BVHTriangle>* firstNode, float& outIntersectionDistance, RayHit& outHitDetails)
{
	float rootEntry, rootExit;
	if (!firstNode->Bounds.DoesRayIntersect(ray, rootEntry, rootExit))
	{
		return;
	}

	LocalStack stack;
	stack.RealStackPointer = &stack.stack[0]-1;

	//Push first node onto stack
	stack.Push(LoopState(firstNode, rootEntry));


	BVH2::BVHNode<BVHTriangle>* currentNode;

	while (stack.HasItems())
	{
		LoopState currentState = stack.Pop();
		currentNode = currentState.NodeTarget;

		//if (currentState.NearHitTime >= outIntersectionDistance)
		{
			//continue;
		}

		//If has children, try all children
		if (currentNode->HasChildren() == false)
		{
			BVH2::BVHNode<BVHTriangle>* nearNode = currentNode + 1;
			BVH2::BVHNode<BVHTriangle>* farNode = &_bvh2[currentNode->RightNode];

			float nearNode_entry, nearNode_exit;
			float farNode_entry, farNode_exit;

			bool nearIntersects = nearNode->Bounds.DoesRayIntersect(ray, nearNode_entry, nearNode_exit);
			bool farIntersects = farNode->Bounds.DoesRayIntersect(ray, farNode_entry, farNode_exit);

			LoopState nearState = LoopState(nearNode, nearNode_entry);
			LoopState farState = LoopState(farNode, farNode_entry);

			if (nearIntersects && farIntersects)
			{
				if (farNode_entry < nearNode_entry)
				{
					//The 2 are flipped, as ordered by this ray, swap themm
					//std::swap(nearNode, farNode);
					//std::swap(nearNode_entry, farNode_entry);
					//std::swap(nearNode_exit, farNode_exit);
					//std::swap(nearState, farState);

					stack.Push(farState);
					stack.Push(nearState);
					continue;
				}
			}

			if (nearIntersects && nearState.NearHitTime < outIntersectionDistance)
			{
				//AttemptRayIntersection_BVH_2_While(ray, nearNode, outIntersectionDistance, outHitDetails);
				stack.Push(nearState);
			}

			if (farIntersects && farState.NearHitTime < outIntersectionDistance)
			{
				//AttemptRayIntersection_BVH_2_While(ray, farNode, outIntersectionDistance, outHitDetails);
				stack.Push(farState);
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
					if (dist < outIntersectionDistance)
					{
						outIntersectionDistance = dist;

						outHitDetails.HitIndex = triangle.Index;
						outHitDetails.Time = dist;
					}
				}
			}
		}
	}
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