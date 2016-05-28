//
// Created by Rafael Campos on 3/26/16.
//

#include "triangleMesh.h"

const float kEpsilon = 1e-3f;

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

    Normal n(0,0,0);
    for (int j = 0; j < numberVertices; j+=3)
    {
        Point p(positions[j], positions[j+1], positions[j+2]);
        Point pp = obj2World->applyP(p);
        vertexPos.push_back(pp);
        normals.push_back(n);
    }

    for (int k = 0; k < faces.size(); ++k)
    {
        Point *pp = &vertexPos[faces[k]];
        vertexIdx.push_back(pp);
        Normal *pn = &normals[faces[k]];
        normalIdx.push_back(pn);
    }

    for (int i = 0; i < faces.size(); i+=3) {
        Point p0 = *(vertexIdx[i]);
        Point p1 = *(vertexIdx[i + 1]);
        Point p2 = *(vertexIdx[i + 2]);

        Normal n = calculateNormal(p0, p1, p2);
        *normalIdx[i] += n;
        *normalIdx[i + 1] += n;
        *normalIdx[i + 2] += n;
    }

    for (int i = 0; i < normals.size(); ++i)
        normals[i].normalize();

    computeBoundingBox();
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

nikita::BoundingBox TriangleMesh::objectBound() const
{
    return *objBBox;
}

nikita::BoundingBox TriangleMesh::worldBound() const
{
    return *worldBBox;
}

void TriangleMesh::computeBoundingBox()
{
    worldBBox = std::make_shared<BoundingBox>(vertexPos[0], vertexPos[0]);
    Point p = (*worldToObject).applyP(vertexPos[0]);
    objBBox = std::make_shared<BoundingBox>(p, p);
    for (int i = 1; i < vertexPos.size(); ++i) {
        *worldBBox = worldBBox->expand(vertexPos[i]);
        *objBBox = objBBox->expand((*worldToObject).applyP(vertexPos[i]));
    }
}

////////////////// Triangle class methods
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

nikita::BoundingBox Triangle::worldBound() const
{
    BoundingBox b(getP1(), getP2());
    return b.expand(getP3());
}

nikita::BoundingBox Triangle::objectBound() const
{
    BoundingBox b((*worldToObject).applyP(getP1()), (*worldToObject).applyP(getP2()));
    return b.expand((*worldToObject).applyP(getP3()));
}

bool Triangle::intersect(const Ray &ray, float *t, IntersectionPtr ip) const
{
    float b1, b2;
    bool hit = calculateIntersection(ray, t, &b1, &b2);

    if (hit) {
        ip->hit = true;
        ip->hitPoint = ray(*t);
        ip->normal = getInterpolatedNormal(b1, b2);
    }

    return hit;
}

bool Triangle::intersectP(const Ray &ray, float *t) const
{
//    std::cout << "Triangle::IntersectP" << std::endl;
    return calculateIntersection(ray, t, nullptr, nullptr);
}


bool Triangle::calculateIntersection(const Ray &ray, float *t, float *bary1, float *bary2) const
{
    // obtain the vertices for this triangle
    Point p1(getP1());
    Point p2(getP2());
    Point p3(getP3());

    Vector e1(p2 - p1);
    Vector e2(p3 - p1);
    Vector s1 = ray.direction.cross(e2);
    float denominator = s1.dot(e1);

    if (MathUtils::almostEqual(0.0, denominator))
        return false;

    float inverseDenom = 1.0f / denominator;

    // Calculating b1, the first barycentric coordinate
    Vector d = ray.origin - p1;
    float b1 = d.dot(s1) * inverseDenom;

    if (bary1) *bary1 = b1;

    if (b1 < 0.0 || b1 > 1.0) // no intersection
        return false;

    // b2, second b. coord.
    Vector s2 = d.cross(e1);
    float b2 = ray.direction.dot(s2) * inverseDenom;

    if (bary2) *bary2 = b2;

    if (b2 < 0.0 || b1 + b2 > 1.0) // no intersection
        return false;

    float hit = e2.dot(s2) * inverseDenom;
    if (hit < ray.tMin || hit > ray.tMax)
        return false;

    if (hit > kEpsilon) {
        if (*t == 0 || hit < *t) {
            *t = hit;
            return true;
        }
    }
    return false;

}

Normal TriangleMesh::calculateNormal(Point p0, Point p1, Point p2)
{
    Vector a(p1 - p0);
    Vector b(p2 - p0);
    Normal n = a.cross(b);
    return (n.normalized());
}

const Normal Triangle::getTriangleNormal() const
{
    Normal n(0,0,0);
    n += mesh->normals[3 * this->triangleNumber];
    n += mesh->normals[3 * this->triangleNumber + 1];
    n += mesh->normals[3 * this->triangleNumber + 2];
    return (n.normalized());
}

const Normal Triangle::getInterpolatedNormal(float u, float v) const
{
    Normal n((1.0f - u - v) * *mesh->normalIdx[3 * this->triangleNumber]
                 + u * *mesh->normalIdx[3 * this->triangleNumber + 1]
                 + v * *mesh->normalIdx[3 * this->triangleNumber + 2]);
    return (n.normalized());
}

