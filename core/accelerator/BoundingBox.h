//
// Created by Rafael Campos on 3/28/16.
//

#ifndef NIKITA_RENDER_BOUNDINGBOX_H
#define NIKITA_RENDER_BOUNDINGBOX_H

#include "../nikita.h"

namespace nikita
{
class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const Point& p1, const Point& p2);
    BoundingBox(const BoundingBox &other);

    Point pMin() const;
    Point pMax() const;
    float pMin(int) const;
    float pMax(int) const;
    void setPoints(const Point& p1, const Point& p2);

    float surfaceArea() const;
    float volume() const;
    int maximumDimension() const;

    bool inside(const Point& p);
    void expand(float delta);
    // Expands to include p in the BBox.
    BoundingBox expand(const Point &p);
    static BoundingBox U(const BoundingBox &b1, const BoundingBox &b2);

private:
    Point lower, upper;

    void validateAndSetPoints(const Point& p1, const Point& p2);
};

typedef std::shared_ptr<BoundingBox> BBoxPtr;

}

#endif //NIKITA_RENDER_BOUNDINGBOX_H
