#pragma once
#include "stdafx.h"
#include <glm.hpp>

using namespace glm;

struct Ray;

struct alignas(16) Triangle
{
	glm::vec3 A;
	float pA;
	glm::vec3 B;
	float pB;
	glm::vec3 C;
	float pC;

	glm::vec3 Normal;

	//TODO:Don't know if we need this but it aligns nice
	float Padding;
	//What else ?

	static Triangle MakeTriangle(glm::ivec3* indicies, glm::vec3* verticies, int index);

	inline bool RayTriangleIntersection(const Ray& localRay, float& outHitDistance);

	static glm::vec3 ComputeByCords(const glm::vec3& vertA,
		const glm::vec3& vertB,
		const glm::vec3& vertC,
		const glm::vec3& hitPoint);
};



struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;

	Ray();

	Ray(const glm::vec3& origin, const glm::vec3& direction);

	Ray Transform(const mat4& matrix);

	glm::vec3 Along(float time);
};

struct FastRay
{
	glm::vec3 Origin;
	glm::vec3 Direction;
	glm::vec3 DirectionReciprocal;

	FastRay();

	FastRay(const glm::vec3& origin, const glm::vec3& direction);
	FastRay(const Ray& source);

	FastRay Transform(const mat4& matrix);

	glm::vec3 Along(float time);
};



inline bool Triangle::RayTriangleIntersection(const Ray& localRay, float& outHitDistance)
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

inline glm::vec3 Triangle::ComputeByCords(const glm::vec3& vertA, const glm::vec3& vertB, const glm::vec3& vertC, const glm::vec3& hitPoint)
{
	glm::vec3 v0 = vertB - vertA, v1 = vertC - vertA, v2 = hitPoint - vertA;
	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;

	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;

	return glm::vec3(u, v, w);
}
