#include "stdafx.h"
#include "MapLoader.h"
#include "FileUtils.h"
#include "World/Mesh.h"

struct MeshFacet
{
    int IndexStart;
    int IndexCount;
};

struct Header
{
    int MeshCount;
    int IndexCount;

    int VertexCount;
    int UVCount;
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
