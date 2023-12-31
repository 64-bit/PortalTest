#include "stdafx.h"
#include "BoundingBox.h"
#include "MathTypes.h"

using namespace glm;

BoundingBox::BoundingBox()
{
	Max = vec3(-INFINITY, -INFINITY, -INFINITY);
	Min = vec3(INFINITY, INFINITY, INFINITY);
}

bool BoundingBox::DoesRayIntersect(const Ray& ray, float& outTNear, float& outTFar)
{

	float tNear = -FLT_MAX;
	float tFar = FLT_MAX;


	for (int i = 0; i < 3; i++)
	{
		float t1 = (Min[i] - ray.Origin[i]) / ray.Direction[i];
		float t2 = (Max[i] - ray.Origin[i]) / ray.Direction[i];

		if (t1 > t2)
		{
			std::swap(t1, t2);
		}

		if (t1 > tNear)
		{
			tNear = t1;
		}

		if (t2 < tFar)
		{
			tFar = t2;
		}
	}

	if (tNear > tFar
		|| tFar < 0.0f)
	{
		return false;
	}

	outTNear = tNear;
	outTFar = tFar;

	return true;
}

bool BoundingBox::DoesRayIntersect(const FastRay& ray, float& outTNear, float& outTFar)
{
	float tNear = -FLT_MAX;
	float tFar = FLT_MAX;


	for (int i = 0; i < 3; i++)
	{
		float t1 = (Min[i] - ray.Origin[i]) * ray.DirectionReciprocal[i];
		float t2 = (Max[i] - ray.Origin[i]) * ray.DirectionReciprocal[i];

		if (t1 > t2)
		{
			std::swap(t1, t2);
		}

		if (t1 > tNear)
		{
			tNear = t1;
		}

		if (t2 < tFar)
		{
			tFar = t2;
		}
	}

	if (tNear > tFar
		|| tFar < 0.0f)
	{
		return false;
	}

	outTNear = tNear;
	outTFar = tFar;

	return true;
}

//void BoundingBox::EnlargeByPoints(const std::vector<MeshVertex>& verticies)
//{
//	//Pick any vertex, and set that as the inital min and max
//	if (verticies.size() > 0)
//	{
//
//		Min = verticies[0].Position;
//		Max = Min;
//
//		for (auto& vertex : verticies)
//		{
//			Min = vec3::Min(Min, vertex.Position);
//			Max = vec3::Max(Max, vertex.Position);
//		}
//	}
//	else
//	{
//		Min = vec3(0.0f, 0.0f, 0.0f);
//		Max = Min;
//	}
//}

void BoundingBox::EnlargeByPoint(const vec3& point)
{
	Min = min(Min, point);
	Max = max(Max, point);
}

//void BoundingBox::EnlargeByTriangles(const std::vector<MeshTriangle>& triangles, const std::vector<MeshVertex>& verticies)
//{
//	for (auto& triangle : triangles)
//	{
//		auto& A = verticies[triangle.A];
//		auto& B = verticies[triangle.B];
//		auto& C = verticies[triangle.C];
//
//		Min = vec3::Min(Min, A.Position);
//		Min = vec3::Min(Min, B.Position);
//		Min = vec3::Min(Min, C.Position);
//
//		Max = vec3::Max(Max, A.Position);
//		Max = vec3::Max(Max, B.Position);
//		Max = vec3::Max(Max, C.Position);
//	}
//}

void BoundingBox::EnlargeByBounds(const BoundingBox& otherBounds)
{
	Min = min(Min, otherBounds.Min);
	Max = max(Max, otherBounds.Max);
}

vec3 BoundingBox::GetCenter() const
{
	return (Min + Max) * 0.5f;
}

glm::vec3 BoundingBox::GetSize() const
{
	return Max - Min;
}

float BoundingBox::SurfaceAreaApprox() const
{
	vec3 size = GetSize();

	return size.x * size.y + size.y * size.z + size.z * size.x;
}
