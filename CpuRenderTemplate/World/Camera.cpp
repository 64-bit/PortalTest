#include "stdafx.h"
#include "Camera.h"
#include <SDL.h>
#include "World.h"

using namespace glm;

const float MOUSE_SENSITIVITY = 200.0f;


Ray Camera::TransformViewRay(const Ray& viewRay)
{
    Ray result;
    result.Origin = viewRay.Origin + Position;
    result.Direction = viewRay.Direction * Rotation;

    return result;
}

void Camera::Move(World& world, float deltaT)
{
	const float CAMERA_SPEED = 3.0f;

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

	if (length(move) > 0)
	{
		Ray moveRay = Ray(Position, normalize(move));
		MoveViaRay(world, moveRay, deltaT * CAMERA_SPEED);
	}	

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

#define MAX_DEPTH 20

void Camera::MoveViaRay(World& world, Ray ray, float remainingDistance, int depth)
{
	
	RayHit hit = world.WorldBVHMesh.RaytraceMesh(ray, remainingDistance);

	if (hit.DidHit())
	{
		

		//do portal test and possibly collision blocking
		int portalIndex = world.PortalIndicies[hit.HitIndex];
		if (portalIndex >= 0)
		{
			if (depth > MAX_DEPTH)
			{
				printf("big error\n");
				return;//TODO: BIG ERROR
			}

			printf("Taking portal %d \n", portalIndex);

			//Transform ray by portal
			mat4 portalTransform = world.Portals[portalIndex];
			//portalTransform = transpose(portalTransform);

			//TODO:Transform camera rotation here
			vec3 forward(0.0f, 0.0f, 1.0f);
			//send forward through portal and use this to figure out new yaw
			forward = portalTransform * vec4(forward, 0.0f);

			float radians = atan2(forward.x, forward.z);
			_yaw += radians;

			const auto newPitch = quat(vec3(_pitch, 0.0f, 0.0f));
			const auto newYaw = quat(vec3(0.0f, _yaw, 0.0f));

			//Rotate camera
			Rotation = newYaw * newPitch; //Apply yaw as global rotation to pitch

			remainingDistance -= hit.Time;

			Ray newRay = ray;
			newRay.Origin = ray.Along(hit.Time);
			newRay.Direction = ray.Direction;

			newRay = newRay.Transform(portalTransform);

			//Recurse
			MoveViaRay(world, newRay, remainingDistance, depth + 1);
			return;
		}
	}

	//move to end of ray
	Position = ray.Along(remainingDistance);
}
