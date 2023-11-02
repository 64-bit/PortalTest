#pragma once
#include "stdafx.h"
#include <stdio.h>
#include "Mesh.h"

class World
{
public:

	void LoadFromFile(FILE* file);

	void Reset();

	Mesh WorldMesh;

	glm::mat4* Portals;
	int* PortalIndicies;

private:




};