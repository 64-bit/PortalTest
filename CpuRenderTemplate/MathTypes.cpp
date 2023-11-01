#include "stdafx.h"

using namespace glm;

Triangle Triangle::MakeTriangle(ivec3* indices, vec3* vertices, int index)
{
    ivec3 triangle = indices[index];

    vec3 a = vertices[triangle.x];
    vec3 b = vertices[triangle.y];
    vec3 c = vertices[triangle.z];

    Triangle result;

    result.A = a;
    result.B = b;
    result.C = c;

    //result.Indicies = triangle;
    result.Normal = cross(b - a, c - a);
    result.Padding = 0;

    return result;
}



Ray::Ray()
{

}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
{
    Origin = origin;
    Direction = direction;
}
