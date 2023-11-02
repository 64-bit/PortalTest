#include "stdafx.h"
#include "MapLoader.h"
#include "FileUtils.h"
#include "World/Mesh.h"
#include "World/World.h"

using namespace glm;

struct MeshFacet
{
    int IndexStart;
    int IndexCount;
};

struct Header
{
    int MeshCount;
    int IndexCount;
    int TriangleCount;

    int VertexCount;
    int UVCount;
    int PortalCount;
};



Mesh MapLoader::LoadMeshFromMapFile(FILE* file)
{
    Header header = ReadTypedObjectFromFile<Header>(file);
    Mesh output;

    output.Surfaces.clear();
    output.Surfaces.reserve(header.MeshCount);

    MeshFacet* facets = new MeshFacet[header.MeshCount];
    ReadTypedArrayFromFile<MeshFacet>(file, header.MeshCount, facets);

    output.Indicies = ReadTypedArrayFromFile<glm::ivec3>(file, header.IndexCount/3);
    output.Verticies = ReadTypedArrayFromFile<glm::vec3>(file, header.VertexCount);
    output.UVs = ReadTypedArrayFromFile<glm::vec2>(file, header.VertexCount);

    output.TriangleCount = header.IndexCount / 3;
    output.Triangles = nullptr;

    output.Rebuild();
    return output;
}

void MapLoader::LoadWorldFromFile(FILE* file, World& world)
{
    world = World();

    Header header = ReadTypedObjectFromFile<Header>(file);
    Mesh output;

    output.Surfaces.clear();
    output.Surfaces.reserve(header.MeshCount);

    MeshFacet* facets = new MeshFacet[header.MeshCount];
    ReadTypedArrayFromFile<MeshFacet>(file, header.MeshCount, facets);

    output.Indicies = ReadTypedArrayFromFile<ivec3>(file, header.IndexCount / 3);
    output.Verticies = ReadTypedArrayFromFile<vec3>(file, header.VertexCount);
    output.UVs = ReadTypedArrayFromFile<vec2>(file, header.VertexCount);

    world.Portals = ReadTypedArrayFromFile<mat4>(file, header.PortalCount);
    world.PortalIndicies = ReadTypedArrayFromFile<int>(file, header.TriangleCount);

    output.TriangleCount = header.IndexCount / 3;
    output.Triangles = nullptr;

    output.Rebuild();

    world.WorldMesh = output;
}