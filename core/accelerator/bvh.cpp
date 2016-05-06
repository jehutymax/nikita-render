//
// Created by Rafael Campos on 5/3/16.
//

#include "bvh.h"

using nikita::BVH;

BVH::BVH(const std::vector<GeoPrimPtr> &prims, unsigned int numLeafPrims)
{
    this->maxPrimsLeaf = numLeafPrims;

    // The divide method expects a vector of pointers to the base class Primitive, so we cast one way....
    std::vector<PrimPtr> baseVec(objects.begin(), objects.end());
    for (int i = 0; i < prims.size(); ++i) {
        // ... call the divide method using pointers to Primitive...
        prims[i]->fullyDivide(baseVec);
    }
    // ... and then cast it back to GeoPrim. No loss of information occurs.
    for (int l = 0; l < baseVec.size(); ++l) {
        objects.push_back(std::dynamic_pointer_cast<GeoPrim>(baseVec[l]));
    }

    std::cout << "BVH will be built for " << objects.size() << " objects." << std::endl;

    if (objects.size() == 0) {
        nodes.clear();
        return;
    }

    // initialize array with all the BVHObjects
    std::vector<BVHObject> data;
    data.reserve(objects.size());

    for (int j = 0; j < objects.size(); ++j) {
        BBoxPtr bp = std::make_shared<BoundingBox>(objects[j]->worldBound());
        data.push_back(BVHObject(j, bp));
    }

    // recursively build the BVH tree
    unsigned int totalNodes = 0;
    std::vector<GeoPrimPtr> orderedObjects;
    orderedObjects.reserve(objects.size());

    BVHNodePtr root = recursiveTreeConstructor(data, 0,
                                               objects.size(),
                                               &totalNodes,
                                               orderedObjects);
    objects.swap(orderedObjects);
    // transform the tree in an array, simulating depth-first traversal
    for (int k = 0; k < totalNodes; ++k) {
        nodes.push_back(std::make_shared<LinearBVHNode>());
    }
    unsigned int offset = 0;
    flattenTree(root, &offset);

    std::cout << "BVH was successfully created, with " << totalNodes << " nodes and " <<  objects.size() << " objects." << std::endl;
}

nikita::BVHNodePtr BVH::recursiveTreeConstructor(std::vector<BVHObject> &objectList,
                                                unsigned int begin,
                                                unsigned int end,
                                                unsigned int *totalNodes,
                                                std::vector<GeoPrimPtr> &orderedObjects)
{
    (*totalNodes)++;
    BVHNodePtr node = std::make_shared<BVHNode>();
    // compute the bounding boxes of all primitives in this node
    BBoxPtr bbox = std::make_shared<BoundingBox>();
    for (int i = begin; i < end; ++i) {
        bbox = std::make_shared<BoundingBox>(BoundingBox::U(*bbox, *objectList[i].bbox));
    }
    unsigned int numberPrimitives = end - begin;
    if (numberPrimitives == 1) {
        // create a leaf node
        unsigned int firstPrimOffset = orderedObjects.size();
        for (unsigned int i = begin; i < end; ++i) {
            unsigned int shapeNum = objectList[i].objectNumber;
            orderedObjects.push_back(objects[shapeNum]);
        }
        node->initLeaf(firstPrimOffset, numberPrimitives, bbox);
    }
    else {
        // compute the bounding boxes of the primitives centroids, and choose the dimension to split on
        BoundingBox centroidBound;
        for (int i = begin; i < end; ++i)
            centroidBound = centroidBound.expand(objectList[i].centroid);
        int dimension = centroidBound.maximumDimension();

        // partition the primitives into two sets, and build the children accordingly
        int midpoint = (begin + end) / 2;
        if (centroidBound.pMax(dimension) == centroidBound.pMin(dimension)) {
            // create leaf node
            // if the remaining primitives fit in a single node, do it here
            if (numberPrimitives <= maxPrimsLeaf)
            {
                unsigned int firstShapeOffset = orderedObjects.size();
                for (int i = begin; i < end; ++i) {
                    unsigned int shapeNum = objectList[i].objectNumber;
                    orderedObjects.push_back(objects[shapeNum]);
                }
                node->initLeaf(firstShapeOffset, numberPrimitives, bbox);
                return node;
            }
            else
            {
                // otherwise, it has to be split again
                node->initInterior(dimension,
                                   recursiveTreeConstructor(objectList, begin, midpoint,
                                                            totalNodes, orderedObjects),
                                   recursiveTreeConstructor(objectList, midpoint, end,
                                                            totalNodes, orderedObjects));
                return node;
            }
        }

        midpoint = (begin + end) / 2;
        std::nth_element(&objectList[begin], &objectList[midpoint], &objectList[end-1]+1,
                         BVHComparePoints(dimension));

        node->initInterior(dimension,
                           recursiveTreeConstructor(objectList, begin, midpoint,
                                                    totalNodes, orderedObjects),
                           recursiveTreeConstructor(objectList, midpoint, end,
                                                    totalNodes, orderedObjects));
    }
    return node;
}

unsigned int BVH::flattenTree(BVHNodePtr node, unsigned int *offset)
{
    LinearNodePtr linearNode = nodes[*offset];
    linearNode->bbox = node->bounds;
    unsigned int myOffset = (*offset)++;

    if (node->numberPrimitives > 0)
    {
        linearNode->primOffset = node->firstShapeOffset;
        linearNode->numberPrimitives = node->numberPrimitives;
    }
    else
    {
        // create an interior node for the flattened tree
        linearNode->axis = node->splitAxis;
        linearNode->numberPrimitives = 0;
        flattenTree(node->children[0], offset);
        linearNode->secondChildOffset = flattenTree(node->children[1], offset);
    }

    return myOffset;
}

bool BVH::intersect(Ray &ray, IntersectionPtr isect)
{
    if (nodes.empty())
        return false;

    bool hit = false;
    Vector directionDivider;
    directionDivider << 1.0f/ray.direction(0), 1.0f/ray.direction(1), 1.0f/ray.direction(2);
    bool directionIsNegative[3] = {directionDivider(0) < 0,
                                   directionDivider(1) < 0,
                                   directionDivider(2) < 0};

    // follow the ray through the tree's nodes to find intersections
    unsigned int toVisitOffset = 0;
    unsigned int nodeNumber = 0;
    unsigned int toVisit[64];
    while(true)
    {
        const LinearNodePtr node = nodes[nodeNumber];
        // check ray against the node
        if (intersectP(node->bbox, ray, directionDivider, directionIsNegative))
        {
            if (node->numberPrimitives > 0)
            {
                // proceed to intersect ray with primitives in leaf
                for (int i = 0; i < node->numberPrimitives; ++i) {
                    if (objects[node->primOffset+i]->intersect(ray, isect))
                        hit = true;
                }
                if (toVisitOffset == 0)
                    break;
                nodeNumber = toVisit[--toVisitOffset];
            }
            else
            {
                // put distant node on stack, advance to next node
                if (directionIsNegative[node->axis])
                {
                    toVisit[toVisitOffset++] = nodeNumber + 1;
                    nodeNumber = node->secondChildOffset;
                }
                else
                {
                    toVisit[toVisitOffset++] = node->secondChildOffset;
                    nodeNumber = nodeNumber + 1;
                }
            }
        }
        else
        {
            if (toVisitOffset == 0)
                break;
            nodeNumber = toVisit[--toVisitOffset];
        }
    }

    return hit;
}

bool BVH::intersectP(const BBoxPtr bbox, const Ray &ray,
                     const Vector &dirDiv, const bool *negativeDirs)
{
    float tMin[3], tMax[3];
    // check ray against planes for x, y and z
    for (int i = 0; i < 3; ++i) {
        if (negativeDirs[i])
        {
            tMin[i] = (bbox->pMax(i) - ray.origin(i)) * dirDiv(i);
            tMax[i] = (bbox->pMin(i) - ray.origin(i)) * dirDiv(i);
        }
        else
        {
            tMin[i] = (bbox->pMin(i) - ray.origin(i)) * dirDiv(i);
            tMax[i] = (bbox->pMax(i) - ray.origin(i)) * dirDiv(i);
        }
    }

    if ((tMin[0] > tMax[1]) || (tMin[1] > tMax[0]))
        return false;

    if (tMin[1] > tMin[0])
        tMin[0] = tMin[1];

    if (tMax[1] < tMax[0])
        tMax[0] = tMax[1];

    if ((tMin[0] > tMax[2]) || (tMin[2] > tMax[0]))
        return false;

    if (tMin[2] > tMin[0])
        tMin[0] = tMin[2];

    if (tMax[2] < tMax[0])
        tMax[0] = tMax[2];

    return (tMin[0] < ray.tMax) && (tMax[0] > ray.tMin);
}

nikita::BoundingBox BVH::worldBound() const
{
    if (nodes.size() > 0)
        return *(nodes[0]->bbox);
    else
        return BoundingBox();
}