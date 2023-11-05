#include "stdafx.h"
#include "BVHMeshTriangles.h"

glm::vec3 BVHTriangle::ComputeCenter() const
{
    return (A + B + C) / 3.0f;
}

BoundingBox BVHTriangle::GetBounds() const
{
	BoundingBox bounds;
	bounds.EnlargeByPoint(A);
	bounds.EnlargeByPoint(B);
	bounds.EnlargeByPoint(C);
	return bounds;
}

glm::vec3 BVHTriangle::GetCenter() const
{
	return (A + B + C) / 3.0f;
}

bool BVHTriangle::RayTriangleIntersection(const Ray& localRay, float& outHitDistance)
{
	const float EPSILON = 0.0000001f;

	vec3 edge1 = B - A;
	vec3 edge2 = C - A;

	vec3 h = cross(localRay.Direction, edge2);
	float a = dot(edge1, h);


	//if (a > -EPSILON && a < EPSILON)
	//{
	//	return false;
	//}

	if (a > EPSILON) //Replace the above with this for backface culling
	{
		return false;
	}

	float f = 1.0f / a;
	vec3 s = localRay.Origin - A;
	float u = f * dot(s, h);
	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	vec3 q = cross(s, edge1);
	float v = f * dot(localRay.Direction, q);
	if (v < 0.0f || u + v > 1.0f)
	{
		return false;
	}

	float t = f * dot(edge2, q);

	if (t > EPSILON) // ray intersection
	{
		outHitDistance = t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
	{
		return false;
	}
}

bool BVHTriangle::RayTriangleIntersection(const FastRay& localRay, float& outHitDistance)
{
	const float EPSILON = 0.0000001f;

	vec3 edge1 = B - A;
	vec3 edge2 = C - A;

	vec3 h = cross(localRay.Direction, edge2);
	float a = dot(edge1, h);


	//if (a > -EPSILON && a < EPSILON)
	//{
	//	return false;
	//}

	if (a > EPSILON) //Replace the above with this for backface culling
	{
		return false;
	}

	float f = 1.0f / a;
	vec3 s = localRay.Origin - A;
	float u = f * dot(s, h);
	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	vec3 q = cross(s, edge1);
	float v = f * dot(localRay.Direction, q);
	if (v < 0.0f || u + v > 1.0f)
	{
		return false;
	}

	float t = f * dot(edge2, q);

	if (t > EPSILON) // ray intersection
	{
		outHitDistance = t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
	{
		return false;
	}
}
