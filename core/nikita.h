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
class Intersection;
typedef std::shared_ptr<Intersection> IntersectionPtr;
}

#endif //NIKITA_RENDER_NIKITA_H
