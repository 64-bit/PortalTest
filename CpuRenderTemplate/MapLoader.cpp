#include "stdafx.h"
#include "MapLoader.h"
#include "FileUtils.h"
#include "World/Mesh.h"
#include "World/World.h"
#include "Graphics/Texture2D.h"

using namespace glm;

struct MeshFacet
{
    int IndexStart;
    int IndexCount;

    char* TexturePath;
    Texture2D* Texture;
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


void ReadFacets(FILE* file, MeshFacet* facets, int count) 
{
    for (int i = 0; i < count; i++)
    {
        MeshFacet newFacet;
        fread(&newFacet.IndexStart, sizeof(int), 2, file);
        
        newFacet.TexturePath = new char[128];

        for (int i = 0; i < 128; i++)
        {
            char c = getc(file);
            newFacet.TexturePath[i] = c;
            if (c == 0)
            {
                break;
            }
        }

        newFacet.Texture = nullptr;
        if (strlen(newFacet.TexturePath) > 0)
        {
            newFacet.Texture = Texture2D::ReadFromFile(newFacet.TexturePath);
        }
        facets[i] = newFacet;
    }

}

Mesh MapLoader::LoadMeshFromMapFile(FILE* file)
{
    Header header = ReadTypedObjectFromFile<Header>(file);
    Mesh output;

    output.Surfaces.clear();
    output.Surfaces.reserve(header.MeshCount);

    MeshFacet* facets = new MeshFacet[header.MeshCount];
    ReadFacets(file, facets, header.MeshCount);
    //TODO:Move loop to read facets
    for (int i = 0; i < header.MeshCount; i++)
    {
        MeshFacet facet = facets[i];

        MeshSurface surface;
        surface.IndexCount = facet.IndexCount;
        surface.IndexStart = facet.IndexStart;
        surface.Texture = facet.Texture;

        output.Surfaces.push_back(surface);
    }


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
    ReadFacets(file, facets, header.MeshCount);
    //TODO:Move loop to read facets
    for (int i = 0; i < header.MeshCount; i++)
    {
        MeshFacet facet = facets[i];

        MeshSurface surface;
        surface.IndexCount = facet.IndexCount;
        surface.IndexStart = facet.IndexStart;
        surface.Texture = facet.Texture;

        output.Surfaces.push_back(surface);
    }

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