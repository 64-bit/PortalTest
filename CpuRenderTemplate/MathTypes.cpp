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

Ray Ray::Transform(const mat4& matrix)
{
    Ray result;

    result.Origin = (matrix * vec4(Origin, 1.0f));
    result.Direction = normalize((matrix * vec4(Direction, 0.0f)));

    return result;
}

glm::vec3 Ray::Along(float time)
{
    return Origin + Direction * time;
}

FastRay::FastRay()
{

}

FastRay::FastRay(const glm::vec3& origin, const glm::vec3& direction)
{
    Origin = origin;
    Direction = direction;
    DirectionReciprocal = 1.0f / Direction;
}

FastRay::FastRay(const Ray& source)
{
    Origin = source.Origin;
    Direction = source.Direction;
    DirectionReciprocal = 1.0f / Direction;
}

FastRay FastRay::Transform(const mat4& matrix)
{
    FastRay result;

    result.Origin = (matrix * vec4(Origin, 1.0f));
    result.Direction = normalize((matrix * vec4(Direction, 0.0f)));
    result.DirectionReciprocal = 1.0f / result.Direction;

    return result;
}

glm::vec3 FastRay::Along(float time)
{
    return Origin + Direction * time;
}
