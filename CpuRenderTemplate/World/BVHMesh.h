#pragma once
#include "DataStructures/BHV.h"
#include "BVHMeshTriangles.h"
#include <vector>
#include "DataStructures/BoundingBox.h"
#include "Mesh.h"

class BVHMesh
{
public:





	BVHMesh();

	BVHMesh(Mesh& source);

	RayHit RaytraceMesh(Ray& ray, float maxDistance = FLT_MAX);

private:

	BVHNode<BVHTriangle>* _bvh;
	BoundingBox _boundingBoxWorldspace;

	bool AttemptRayIntersection_BVH(const Ray& ray, BVHNode<BVHTriangle>* currentNode,
		float entry, float exit, float& outIntersectionDistance, RayHit& outHitDetails);

	RayHit GetSurfaceForHit(const Ray& ray, float hitDistance, const BVHTriangle& hitTriangle);
};