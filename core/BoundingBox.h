//
// Created by Rafael Campos on 3/28/16.
//

#ifndef NIKITA_RENDER_BOUNDINGBOX_H
#define NIKITA_RENDER_BOUNDINGBOX_H

#include "nikita.h"

namespace nikita
{
class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(Point& p1, Point& p2);

    Point pMin() const;
    Point pMax() const;
    float pMin(int) const;
    float pMax(int) const;
    void setPoints(const Point& p1, const Point& p2);

    float surfaceArea() const;
    float volume() const;

    bool inside(const Point& p);
    void expand(float delta);

private:
    Point lower, upper;

    void validateAndSetPoints(const Point& p1, const Point& p2);
};
}

#endif //NIKITA_RENDER_BOUNDINGBOX_H
