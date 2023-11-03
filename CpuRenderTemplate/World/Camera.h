#pragma once
#include "stdafx.h"

class World;

class Camera
{
public:

	glm::vec3 Position;
	glm::quat Rotation;
	float fovY;

	Ray TransformViewRay(const Ray& viewRay);

	void Move(World& world, float deltaT);
private:
	int _lastMouseX;
	int _lastMouseY;

	float _pitch = 0.0f;
	float _yaw = 0.0f;

	void MoveViaRay(World& world, Ray ray, float remainingDistance, int depth = 0);
};