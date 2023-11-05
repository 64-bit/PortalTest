#pragma once
#include <glm.hpp>
#include "DataStructures/BoundingBox.h"

struct BVHTriangle
{
	glm::vec3 A;
	float pA;
	glm::vec3 B;
	float pB;
	glm::vec3 C;
	//float pad;
	int Index;

	glm::vec4 PadV;

	//glm::vec3 Center;

	//glm::vec2 UVs[3];

	glm::vec3 ComputeCenter() const;

	BoundingBox GetBounds() const;
	glm::vec3 GetCenter() const;

	bool RayTriangleIntersection(const Ray& localRay, float& outHitDistance);
	bool RayTriangleIntersection(const FastRay& localRay, float& outHitDistance);

};

struct NodeTriangle
{
	glm::vec3 A;
	float pA;
	glm::vec3 B;
	float pB;
	glm::vec3 C;
	float pC;

	glm::vec3 Normal;
	float pNorm;
};

struct AttributeTriangle
{
	glm::vec2 UVs [3];
	//TODO: normals ?
	//Material index
	//Object index
};