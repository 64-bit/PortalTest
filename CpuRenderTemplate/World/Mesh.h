#pragma once
#include "stdafx.h"



class MeshSurface
{
public:

	int IndexCount;
	int TriangleCount;
	int VertexCount;

	glm::vec3* Indicies;
	glm::vec3* Verticies;
	glm::vec2* UVs;

	int MaterialID;

private:
};

class Mesh
{
public:

	int IndexCount;
	int TriangleCount;
	int VertexCount;

	int* Indicies;
	glm::vec3* Verticies;
	glm::vec2* UVs;

	std::vector<MeshSurface> Surfaces;

private:
};