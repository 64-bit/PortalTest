#include "stdafx.h"
#include "World.h"
#include "MapLoader.h"

void World::LoadFromFile(FILE* file)
{
	WorldMesh = MapLoader::LoadMeshFromMapFile(file);
}

void World::Reset()
{

}
