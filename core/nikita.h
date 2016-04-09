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

#define PI 3.1415927

typedef Eigen::Vector3f Point;
typedef Eigen::Vector3f Vector;
typedef Eigen::Matrix4f Matrix;

static constexpr float Infinity = std::numeric_limits<float>::infinity();

#endif //NIKITA_RENDER_NIKITA_H
