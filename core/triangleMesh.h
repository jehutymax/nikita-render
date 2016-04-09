//
// Created by Rafael Campos on 3/26/16.
//

#ifndef NIKITA_RENDER_TRIANGLEMESH_H
#define NIKITA_RENDER_TRIANGLEMESH_H

#include "shape.h"
#include <eigen3/Eigen/Geometry> // for the cross product.

namespace nikita
{
    class TriangleMesh : public Shape
    {
    public:
        TriangleMesh(const TransformPtr obj2World,
                     const TransformPtr world2Obj,
                     int nTriangles, int nVertices,
                     std::vector<float> positions,
                     std::vector<int> faces);

        bool isIntersectable();
        bool divide(std::vector<ShapePtr> &divided);

        int numberTriangles;
        int numberVertices;
        std::vector<Point*> vertexIdx;
        std::vector<Point> vertexPos;
        //todo: later store normals
        std::vector<ShapePtr> triangles;
    };

    class Triangle : public Shape
    {
    public:
        Triangle(const TransformPtr obj2world, const TransformPtr world2obj,
                    TriangleMesh* parent, int id);

        bool isIntersectable();
        bool intersect(const Ray &ray, float *t) const;

        inline const Point& getP1() const;
        inline const Point& getP2() const;
        inline const Point& getP3() const;
    private:
        TriangleMesh *mesh;
        int triangleNumber;

    };

    typedef std::shared_ptr<TriangleMesh> TriangleMeshPtr;
    typedef std::shared_ptr<Triangle> TrianglePtr;
}

#endif //NIKITA_RENDER_TRIANGLEMESH_H
