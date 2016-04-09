//
// Created by Rafael Campos on 3/26/16.
//

#ifndef NIKITA_RENDER_TRANSFORM_H
#define NIKITA_RENDER_TRANSFORM_H

#include "nikita.h"
#include "ray.h"
#include "../util/mathUtils.h"
#include "eigen3/Eigen/Geometry" // for cross-product!
#include "eigen3/Eigen/LU" // for determinant.

namespace nikita
{
    class Transform
    {
    public:
        Transform();
        Transform(const Matrix& t);
        Transform(const Matrix& t, const Matrix& inverse);

        typedef Eigen::Vector4f HPoint;

        const static Transform getTranslation(const Vector& translation);
        const static Transform getUniformScale(float scale);
        const static Transform getScale(const Vector& scale);
        const static Transform getRotationX(float angle);
        const static Transform getRotationY(float angle);
        const static Transform getRotationZ(float angle);
        const static Transform getRotation(float angle, const Vector& axis);
        const static Transform getLookAt(const Point& pos, const Point& lookAt, const Vector& up);
        const static Transform getPerspective(float fieldOfView, float n, float f);

        bool affectsHandedness();

        const Matrix get() const;
        const Matrix getInv() const;

        // Applying the transformation
        inline Point applyP(const Point &p) const;
        inline Vector applyV (const Vector& v) const;
        inline Ray operator()(const Ray &ray) const;

        Transform operator*(const Transform& t) const;

    private:
        Matrix transformation;
        Matrix inverseTransformation;

        inline static Matrix createMatrixInstance(bool setIdent = true);
        inline static void fillColumn(Matrix& m, const Vector& v, int col);
        inline static HPoint getHomogeneousPoint(const Point& p);
        inline static HPoint getHomogeneousVector(const Vector& v);
        inline static Point getPoint(const HPoint& hp);
        inline static Vector getVector(const HPoint& hv);
    };

    typedef std::shared_ptr<Transform> TransformPtr;
}

using nikita::Transform;

// Inline definitions
inline Point nikita::Transform::applyP(const Point &p) const
{
    HPoint result, hp = getHomogeneousPoint(p); // hp = homogeneous point
    result = this->transformation * hp;
    return getPoint(result);
}

inline Vector nikita::Transform::applyV(const Vector &v) const
{
    HPoint result, hv = getHomogeneousVector(v); // hp = homogeneous vector
    result = this->transformation * hv;
    return getVector(result);
}

inline nikita::Ray nikita::Transform::operator()(const Ray &ray) const
{
    Ray r = ray;
    Point origin = applyP(ray.origin);
    Point direction = applyV(ray.direction);
    r.origin = origin;
    r.direction = direction;
    return r;
}

inline Matrix nikita::Transform::createMatrixInstance(bool setIdentity)
{
    Matrix m;
    if (setIdentity)
        m.setIdentity();
    else
        m.setZero();
    return m;
}

inline void nikita::Transform::fillColumn(Matrix& m, const Vector& v, int col)
{
    m(0, col) = v(0);
    m(1, col) = v(1);
    m(2, col) = v(2);
}

inline nikita::Transform::HPoint nikita::Transform::getHomogeneousPoint(const Point& p)
{
    return HPoint(p(0), p(1), p(2), 1.0);
}

inline nikita::Transform::HPoint nikita::Transform::getHomogeneousVector(const Vector& v)
{
    return HPoint(v(0), v(1), v(2), 0.0);
}

inline Point nikita::Transform::getPoint(const HPoint& hp)
{
    if (!MathUtils::almostEqual(1.0, hp(3)))
        return Point(hp(0)/hp(3), hp(1)/hp(3), hp(2)/hp(3));
    else return Point(hp(0), hp(1), hp(2));
}

inline Vector nikita::Transform::getVector(const HPoint& hv)
{
    return Vector(hv(0), hv(1), hv(2));
}

#endif //NIKITA_RENDER_TRANSFORM_H
