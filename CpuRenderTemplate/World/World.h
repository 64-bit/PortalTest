#pragma once
#include "stdafx.h"
#include <stdio.h>
#include "Mesh.h"
#include "BVHMesh.h"

class Texture2D;

class World
{
public:

	void LoadFromFile(FILE* file);

	void Reset();

	Mesh WorldMesh;
	BVHMesh WorldBVHMesh;

	glm::mat4* Portals;
	int* PortalIndicies;


	Texture2D* Texture;

private:



};