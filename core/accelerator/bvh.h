//
// Created by Rafael Campos on 5/3/16.
//

#ifndef NIKITA_RENDER_BVH_H
#define NIKITA_RENDER_BVH_H

#include <vector>
#include "../primitive.h"
namespace nikita
{
struct BVHObject;
struct BVHNode;
struct LinearBVHNode;
struct BVHCompare;
struct BVHComparePoints;

typedef std::shared_ptr<BVHNode> BVHNodePtr;
typedef std::shared_ptr<LinearBVHNode> LinearNodePtr;

class BVH : public Primitive
{
public:
    BVH(const std::vector<GeoPrimPtr>&prims, uint32_t numLeafPrims);
    bool intersect(Ray &ray, IntersectionPtr isect);
    static inline bool intersectP(const BBoxPtr bbox,
                                  const Ray &ray,
                                  const Vector &dirDiv,
                                  const bool negativeDirs[3]);


    virtual BoundingBox worldBound() const;
    virtual bool isIntersectable() const {return true;};
    virtual bool divide(std::vector<PrimPtr> &divided) const {return false;};

private:
    uint32_t maxPrimsLeaf;
    std::vector<GeoPrimPtr> objects;
    std::vector<LinearNodePtr> nodes;

    BVHNodePtr recursiveTreeConstructor(std::vector<BVHObject> &objectList,
                                      unsigned int begin,
                                      unsigned int end,
                                      unsigned int *totalNodes,
                                      std::vector<GeoPrimPtr> &orderedObjects);
    unsigned int flattenTree(BVHNodePtr node, unsigned int *offset);
};

struct BVHObject
{
    BVHObject(int objNumber, const BBoxPtr bounds)
        : objectNumber(objNumber), bbox(bounds)
    {
        centroid = 0.5f * bounds->pMin() + 0.5f * bounds->pMax();
    }

    Point centroid;
    BBoxPtr bbox;
    int objectNumber;
};

struct BVHNode
{
    // methods
    BVHNode() {
        children[0] = children[1] = nullptr;
    }
    // this node can be initialized as an interior node or as a leaf node
    void initLeaf(int firstShape, int numShapes, BBoxPtr bbox)
    {
        firstShapeOffset = firstShape;
        numberPrimitives = numShapes;
        bounds = std::make_shared<BoundingBox>(*bbox);
    }

    void initInterior(int axis, BVHNodePtr child0, BVHNodePtr child1)
    {
        children[0] = child0;
        children[1] = child1;
        bounds = std::make_shared<BoundingBox>(BoundingBox::U(*child0->bounds, *child1->bounds));
        splitAxis = axis;
        numberPrimitives = 0;
    }

    // properties
    BBoxPtr bounds;
    BVHNodePtr children[2]; // BVH is a binary tree
    unsigned int splitAxis;
    unsigned int firstShapeOffset;
    unsigned int numberPrimitives;
};

struct LinearBVHNode {
    BBoxPtr bbox;
    union {
        unsigned int primOffset; // for leaf
        unsigned int secondChildOffset; // for interior
    };
    unsigned int numberPrimitives; // when 0, it's an interior node
    unsigned int axis; // for interior node, can be any of xyz
};

struct BVHCompare {
    BVHCompare(int dim, float center) {
        dimension = dim;
        midpoint = center;
    }

    bool operator()(const BVHObject &o) const {
        return o.centroid[dimension] < midpoint;
    }

    int dimension;
    float midpoint;
};

struct BVHComparePoints
{
    BVHComparePoints(int dim) { dimension = dim; }
    bool operator()(const BVHObject &obj1, const BVHObject &obj2) const
    {
        return obj1.centroid[dimension] < obj2.centroid[dimension];
    }

    int dimension;
};
}

#endif //NIKITA_RENDER_BVH_H
