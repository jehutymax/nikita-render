//
// Created by Rafael Campos on 3/28/16.
//

#include "BoundingBox.h"

nikita::BoundingBox::BoundingBox()
{
    lower << Eigen::Infinity, Eigen::Infinity, Eigen::Infinity;
    upper << -Eigen::Infinity, -Eigen::Infinity, -Eigen::Infinity;
}

nikita::BoundingBox::BoundingBox(Point& p1, Point& p2)
{
    validateAndSetPoints(p1, p2);
}

Point nikita::BoundingBox::pMin() const
{
    return lower;
}

Point nikita::BoundingBox::pMax() const
{
    return upper;
}

float nikita::BoundingBox::pMin(int i) const
{
    assert(i >= 0 && i <=2 );
    return lower(i);
}

float nikita::BoundingBox::pMax(int i) const
{
    assert(i >= 0 && i <=2 );
    return upper(i);
}

void nikita::BoundingBox::setPoints(const Point &p1, const Point &p2)
{
    validateAndSetPoints(p1, p2);
}

float nikita::BoundingBox::surfaceArea() const
{
    Vector diff = pMax() - pMin();
    return (2 * (diff(0) * diff(1) + diff(0) * diff(2) + diff(1) * diff(2)));
}

float nikita::BoundingBox::volume() const
{
    Vector diff = pMax() - pMin();
    return (diff(0) * diff(1) * diff(2));
}

bool nikita::BoundingBox::inside(const Point& p)
{
    for (int i = 0; i < 3; i++)
        if (!(p(i) >= pMin(i) && p(i) <= pMax(i)))
            return false;
    return true;
}

void nikita::BoundingBox::expand(float delta)
{
    Point d(delta, delta, delta);
    setPoints(pMin() - d, pMax() + d);
}

void nikita::BoundingBox::validateAndSetPoints(const Point &p1, const Point &p2)
{
    for (int i = 0; i < 3; ++i)
    {
        lower(i) = std::min(p1(i), p2(i));
        upper(i) = std::max(p1(i), p2(i));
    }
}