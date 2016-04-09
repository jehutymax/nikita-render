//
// Created by Rafael Campos on 3/26/16.
//

#include "triangleMesh.h"

using nikita::TriangleMesh;
using nikita::Triangle;

TriangleMesh::TriangleMesh(const TransformPtr obj2World,
                           const TransformPtr world2Obj,
                           int nTriangles, int nVertices,
                           std::vector<float> positions,
                           std::vector<int> faces)
    : Shape(obj2World, world2Obj)
{
    numberTriangles = nTriangles;
    numberVertices = nVertices;

    for (int j = 0; j < numberVertices; j+=3)
    {
        Point p(positions[j], positions[j+1], positions[j+2]);
        Point pp = obj2World->applyP(p);
        vertexPos.push_back(pp);
    }

    for (int k = 0; k < faces.size(); ++k)
    {
        Point *pp = &vertexPos[faces[k]];
        vertexIdx.push_back(pp);
    }
}

bool TriangleMesh::isIntersectable()
{
    return false;
}

bool TriangleMesh::divide(std::vector<ShapePtr> &divided)
{
    if (triangles.size() == 0) {
        for (int i = 0; i < numberTriangles; ++i) {
            triangles.push_back(std::make_shared<Triangle>(objectToWorld, worldToObject, (TriangleMesh *) this, i));
        }
    }
    divided = triangles;
    return true;
}

// Triangle class methods
Triangle::Triangle(const TransformPtr obj2world, const TransformPtr world2obj, TriangleMesh *parent, int id)
    : Shape(obj2world, world2obj)
{
    this->mesh = parent;
    this->triangleNumber = id;
}

bool Triangle::isIntersectable()
{
    return true;
}

const Point& Triangle::getP1() const
{
    return *mesh->vertexIdx[3 * this->triangleNumber];
}

const Point& Triangle::getP2() const
{
    return *mesh->vertexIdx[3 * this->triangleNumber + 1];
}

const Point& Triangle::getP3() const
{
    return *mesh->vertexIdx[3 * this->triangleNumber + 2];
}

bool Triangle::intersect(const Ray &ray, float *t) const
{
    // obtain the vertices for this triangle
    const Point &p1 = getP1();
    const Point &p2 = getP2();
    const Point &p3 = getP3();

    Vector e1 = p2 - p1;
    Vector e2 = p3 - p1;
    Vector s1 = ray.direction.cross(e2);
    float denominator = s1.dot(e1);

    if (MathUtils::almostEqual(0.0, denominator))
        return false;

    float inverseDenom = 1.0f / denominator;

    // Calculating b1, the first barycentric coordinate
    Vector d = ray.origin - p1;
    float b1 = d.dot(s1) * inverseDenom;
    if (b1 < 0.0 || b1 > 1.0) // no intersection
        return false;

    // b2, second b. coord.
    Vector s2 = d.cross(e1);
    float b2 = ray.direction.dot(s2) * inverseDenom;
    if (b2 < 0.0 || b1 + b2 > 1.0) // no intersection
        return false;

    float hit = e2.dot(s2) * inverseDenom;
    if (hit < ray.tMin || hit > ray.tMax)
        return false;

    *t = hit;
    return true;
}