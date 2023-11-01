#include "stdafx.h"
#include "Camera.h"
#include <SDL.h>

using namespace glm;

const float MOUSE_SENSITIVITY = 200.0f;


Ray Camera::TransformViewRay(const Ray& viewRay)
{
    Ray result;
    result.Origin = viewRay.Origin + Position;
    result.Direction = viewRay.Direction * Rotation;

    return result;
}



void Camera::Move(float deltaT)
{
	const float CAMERA_SPEED = 50.0f;

    //Translate
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	vec3 move(0.0f, 0.0f, 0.0f);

	vec3 forwards = vec3(0.0f, 0.0f, -1.0f);
	vec3 left = vec3(1.0f, 0.0f, 0.0f);

	forwards = Rotation * forwards;
	left = Rotation * left;

	if (state[SDL_SCANCODE_W])
	{
		move += forwards;
	}
	if (state[SDL_SCANCODE_S])
	{
		move -= forwards;
	}
	if (state[SDL_SCANCODE_A])
	{
		move -= left;
	}
	if (state[SDL_SCANCODE_D])
	{
		move += left;
	}
	if (state[SDL_SCANCODE_Q])
	{
		move += vec3(0.0f, -1.0f, 0.0f);
	}
	if (state[SDL_SCANCODE_E])
	{
		move += vec3(0.0f, 1.0f, 0.0f);
	}

	move = move * deltaT * CAMERA_SPEED;
	Position += move;
	

	//Rotate
	int currentMouseX, currentMouseY;
	const auto buttonState = SDL_GetMouseState(&currentMouseX, &currentMouseY);

	bool didCameraMove = false;

	if ((buttonState & 0x1) > 0)
	{
		if (currentMouseX != _lastMouseX ||
			currentMouseY != _lastMouseY)
		{
			didCameraMove = true;

			int deltaX = currentMouseX - _lastMouseX;
			int deltaY = currentMouseY - _lastMouseY;

			float pitchMove = (float)deltaY / MOUSE_SENSITIVITY;
			float yawMove = (float)deltaX / MOUSE_SENSITIVITY;

			_pitch -= pitchMove;
			_yaw -= yawMove;

			const auto newPitch = quat(vec3(_pitch, 0.0f, 0.0f));
			const auto newYaw = quat(vec3(0.0f, _yaw, 0.0f));

			//Rotate camera
			Rotation = newYaw * newPitch; //Apply yaw as global rotation to pitch
		}
	}

	_lastMouseX = currentMouseX;
	_lastMouseY = currentMouseY;
}
