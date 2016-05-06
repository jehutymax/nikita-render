//
// Created by Rafael Campos on 3/28/16.
//

#include "BoundingBox.h"

nikita::BoundingBox::BoundingBox()
{
    lower << Infinity, Infinity, Infinity;
    upper << -Infinity, -Infinity, -Infinity;
}

nikita::BoundingBox::BoundingBox(const Point& p1, const Point& p2)
{
    validateAndSetPoints(p1, p2);
}

nikita::BoundingBox::BoundingBox(const BoundingBox &other)
    : lower(other.lower), upper(other.upper)
{
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

int nikita::BoundingBox::maximumDimension() const
{
    Vector diagonal = upper - lower;
    if (diagonal(0) > diagonal(1) && diagonal(0) > diagonal(2))
        return 0; // X
    else if (diagonal(1) > diagonal(2))
        return 1; // Y
    else
        return 2; // Z
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

nikita::BoundingBox nikita::BoundingBox::expand(const Point &p)
{
    Point min;
    Point max;

    min << std::min(lower(0), p(0)), std::min(lower(1), p(1)), std::min(lower(2), p(2));
    max << std::max(upper(0), p(0)), std::max(upper(1), p(1)), std::max(upper(2), p(2));

    return BoundingBox(min, max);
}

void nikita::BoundingBox::validateAndSetPoints(const Point &p1, const Point &p2)
{
    for (int i = 0; i < 3; ++i)
    {
        lower(i) = std::min(p1(i), p2(i));
        upper(i) = std::max(p1(i), p2(i));
    }
}

nikita::BoundingBox nikita::BoundingBox::U(const BoundingBox &b1, const BoundingBox &b2)
{
    Point min;
    Point max;
    min << std::min(b1.lower(0), b2.lower(0)), std::min(b1.lower(1), b2.lower(1)), std::min(b1.lower(2), b2.lower(2));
    max << std::max(b1.upper(0), b2.upper(0)), std::max(b1.upper(1), b2.upper(1)), std::max(b1.upper(2), b2.upper(2));

    return BoundingBox(min, max);
}