#pragma once
#include <stdio.h>

class Mesh;

class MapLoader
{
public:
	static Mesh LoadMeshFromMapFile(FILE* file);
};