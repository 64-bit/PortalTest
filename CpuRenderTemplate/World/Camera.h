#pragma once
#include "stdafx.h"

class Camera
{
public:

	glm::vec3 Position;
	glm::quat Rotation;
	float fovY;

	Ray TransformViewRay(const Ray& viewRay);

	void Move(float deltaT);
private:
	int _lastMouseX;
	int _lastMouseY;

	float _pitch = 0.0f;
	float _yaw = 0.0f;
};