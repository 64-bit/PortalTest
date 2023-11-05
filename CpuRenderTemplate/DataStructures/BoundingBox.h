#pragma once
#include <glm.hpp>
#include <vector>

struct Ray;

class BoundingBox
{
public:
	glm::vec3 Min;
	glm::vec3 Max;

	BoundingBox();

	bool DoesRayIntersect(const Ray& ray, float& outTNear, float& outTFar);
	bool DoesRayIntersect(const FastRay& ray, float& outTNear, float& outTFar);

	//void EnlargeByPoints(const std::vector<MeshVertex>& verticies);
	void EnlargeByPoint(const glm::vec3& point);

	//void EnlargeByTriangles(const std::vector<MeshTriangle>& triangles, const std::vector<MeshVertex>& verticies);

	void EnlargeByBounds(const BoundingBox& otherBounds);

	glm::vec3 GetCenter() const;

	glm::vec3 GetSize() const;

private:
};