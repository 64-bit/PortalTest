#pragma once
#include <stdio.h>

class Mesh;
class World;

class MapLoader
{
public:
	static Mesh LoadMeshFromMapFile(FILE* file);

	static void LoadWorldFromFile(FILE* file, World& world);
};