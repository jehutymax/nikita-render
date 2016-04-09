//
// Created by Rafael Campos on 4/6/16.
//

#include "transform.h"

using nikita::Transform;

nikita::Transform::Transform()
{
    transformation.setIdentity();
    inverseTransformation.setIdentity();
}

nikita::Transform::Transform(const Matrix &t)
    : transformation(t),
      inverseTransformation(t.inverse())
{
}

nikita::Transform::Transform(const Matrix& t, const Matrix& inverse)
    : transformation(t),
      inverseTransformation(inverse)
{
}

const Transform nikita::Transform::getTranslation(const Vector &translation)
{
    Matrix t = createMatrixInstance();
    t(0, 3) = translation(0);
    t(1, 3) = translation(1);
    t(2, 3) = translation(2);
    return Transform(t);
}

const Transform nikita::Transform::getUniformScale(float scale)
{
    Matrix t = createMatrixInstance();
    t(0, 0) = scale;
    t(1, 1) = scale;
    t(2, 2) = scale;
    return Transform(t);
}

const Transform nikita::Transform::getScale(const Vector &scale)
{
    Matrix t = createMatrixInstance();
    t(0, 0) = scale(0);
    t(1, 1) = scale(1);
    t(2, 2) = scale(2);
    return Transform(t);
}

const Transform nikita::Transform::getRotationX(float angle)
{
    Matrix t = createMatrixInstance();
    float sine = sinf(MathUtils::toRadians(angle));
    float cosine = cosf(MathUtils::toRadians(angle));

    t(1, 1) = cosine;
    t(1, 2) = -sine;
    t(2, 1) = sine;
    t(2, 2) = cosine;
    return Transform(t);
}

const Transform nikita::Transform::getRotationY(float angle)
{
    Matrix t = createMatrixInstance();
    float sine = sinf(MathUtils::toRadians(angle));
    float cosine = cosf(MathUtils::toRadians(angle));

    t(0, 0) = cosine;
    t(0, 2) = sine;
    t(2, 0) = -sine;
    t(2, 2) = cosine;
    return Transform(t);}

const Transform nikita::Transform::getRotationZ(float angle)
{
    Matrix t = createMatrixInstance();
    float sine = sinf(MathUtils::toRadians(angle));
    float cosine = cosf(MathUtils::toRadians(angle));

    t(0, 0) = cosine;
    t(0, 1) = -sine;
    t(1, 0) = sine;
    t(1, 1) = cosine;
    return Transform(t);
}

const Transform nikita::Transform::getLookAt(const Point &pos, const Point &lookAt, const Vector &up)
{
    Matrix t = createMatrixInstance();
    // Translation is the position itself.
    t(0, 3) = pos(0);
    t(1, 3) = pos(1);
    t(2, 3) = pos(2);

    // +z axis is defined as the difference between the point we're looking at and the camera position.
    Vector direction = (lookAt - pos).normalized();
    // Left vector, since this is a right-hand system.
    Vector left = (up.normalized().cross(direction)).normalized();
    // And we want to end up with an orthonormal basis, so we recompute the up vector.
    Vector updatedUp = direction.cross(left);

    fillColumn(t, left, 0);
    fillColumn(t, updatedUp, 1);
    fillColumn(t, direction, 2);

    return Transform(t);
}

const Transform nikita::Transform::getPerspective(float fieldOfView, float n, float f)
{
    // evaluate the projection division
    Matrix t = createMatrixInstance(false);
    t(0, 0) = 1.f;
    t(1, 1) = 1.f;
    t(2, 2) = f / (f - n);
    t(2, 3) = - f * n / (f - n);
    t(3, 2) = 1.f;

    // scale to canonical view volume
    float inverseTan = 1.f / tanf(MathUtils::toRadians(fieldOfView) / 2.0f);
    Vector s(inverseTan, inverseTan, 1);
    return (getScale(s) * Transform(t));
}

Transform nikita::Transform::operator*(const Transform &t) const
{
    Matrix prod = this->transformation * t.transformation;
    Matrix invProd = t.inverseTransformation * this->inverseTransformation;
    return Transform(prod, invProd);
}

bool nikita::Transform::affectsHandedness()
{
    return (this->transformation.topLeftCorner(3, 3).determinant() < 0.0);
}

const Matrix nikita::Transform::get() const
{
    return transformation;
}

const Matrix nikita::Transform::getInv() const
{
    return inverseTransformation;
}