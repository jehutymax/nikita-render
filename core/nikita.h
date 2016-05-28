//
// Created by Rafael Campos on 3/26/16.
//

#ifndef NIKITA_RENDER_NIKITA_H
#define NIKITA_RENDER_NIKITA_H

// Global configuration, with global includes
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <eigen3/Eigen/Dense>  // for matrix, vectors and math operations.

typedef Eigen::Vector3f Point;
typedef Eigen::Vector3f Vector;
typedef Eigen::Vector3f Normal;
typedef Eigen::Matrix4f Matrix;
typedef Eigen::Array3f RGB;

static constexpr float Infinity = std::numeric_limits<float>::infinity();
static constexpr float Pi = 3.1415927;
static constexpr float InvPi = 1.0f/Pi;

namespace nikita
{
// forward declarations
class Primitive;
class Material;
class Intersection;
class Shape;
class Renderer;
class GeoPrim;
typedef std::shared_ptr<GeoPrim> GeoPrimPtr;
typedef std::shared_ptr<Shape> ShapePtr;
typedef std::shared_ptr<Intersection> IntersectionPtr;
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<Primitive> PrimPtr;
typedef std::shared_ptr<Renderer> RendererPtr;
}

#endif //NIKITA_RENDER_NIKITA_H
