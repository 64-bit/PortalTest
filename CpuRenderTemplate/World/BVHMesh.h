#pragma once
#include "DataStructures/BHV.h"
#include "BVHMeshTriangles.h"
#include <vector>
#include "DataStructures/BoundingBox.h"
#include "Mesh.h"

#include "DataStructures/BVH2.h"

class BVHMesh
{
public:





	BVHMesh();

	BVHMesh(Mesh& source);

	RayHit RaytraceMesh(Ray& ray, float maxDistance = FLT_MAX);

	BVHTriangle GetTriangle(int index) const;

private:

	BVHNode<BVHTriangle>* _bvh;
	BoundingBox _boundingBoxWorldspace;

	std::vector<BVH2::BVHNode<BVHTriangle>> _bvh2;
	std::vector<BVHTriangle> _triangles;

	bool AttemptRayIntersection_BVH(const Ray& ray, BVHNode<BVHTriangle>* currentNode,
		float entry, float exit, float& outIntersectionDistance, RayHit& outHitDetails);

	RayHit GetSurfaceForHit(const Ray& ray, float hitDistance, const BVHTriangle& hitTriangle);


	bool AttemptRayIntersection_BVH_2(const FastRay& ray, BVH2::BVHNode<BVHTriangle>* currentNode,
		float& outIntersectionDistance, RayHit& outHitDetails);

	RayHit GetSurfaceForHit_2(const Ray& ray, float hitDistance, const BVHTriangle& hitTriangle);
};