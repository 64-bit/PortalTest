#include "stdafx.h"
#include "World.h"
#include "MapLoader.h"

void World::LoadFromFile(FILE* file)
{
	MapLoader::LoadWorldFromFile(file, *this);
	WorldBVHMesh = BVHMesh(WorldMesh);
}

void World::Reset()
{

}
