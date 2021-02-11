//
//  transformation.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 04.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/matrix.h>

#include <stack>


namespace sunray
{
  class Transformation
  {
  public:
    Transformation() = default;

    ~Transformation() = default;

    Transformation(const Transformation&) = delete;
    Transformation(Transformation&&) = delete;
    Transformation& operator=(const Transformation&) = delete;
    Transformation& operator=(Transformation&&) = delete;

    void clear()
    {
      while (!matrices_.empty()) {
        matrices_.pop();
      }
    }

    Matrix44 matrix()
    {
      if (matrices_.empty()) {
        throw std::runtime_error{"Transformation cannot process empty stack"};
      }

      while (matrices_.size() >= 2) {
        const Matrix44 m1 = matrices_.top();
        matrices_.pop();
        const Matrix44 m2 = matrices_.top();
        matrices_.pop();
        matrices_.emplace(m1 * m2);
      }

      return matrices_.top();
    }

    Transformation& identity()
    {
      matrices_.emplace(Matrix44::identity());
      return *this;
    }

    Transformation& translate(double x, double y, double z)
    {
      matrices_.emplace(Matrix44::translation(x, y, z));
      return *this;
    }

    Transformation& scale(double x, double y, double z)
    {
      matrices_.emplace(Matrix44::scaling(x, y, z));
      return *this;
    }

    Transformation& rotate_x(double radians)
    {
      matrices_.emplace(Matrix44::rotation_x(radians));
      return *this;
    }

    Transformation& rotate_y(double radians)
    {
      matrices_.emplace(Matrix44::rotation_y(radians));
      return *this;
    }

    Transformation& rotate_z(double radians)
    {
      matrices_.emplace(Matrix44::rotation_z(radians));
      return *this;
    }

    Transformation& shear(double xy, double xz, double yx, double yz, double zx, double zy)
    {
      matrices_.emplace(Matrix44::shearing(xy, xz, yx, yz, zx, zy));
      return *this;
    }

  private:
    std::stack<Matrix44> matrices_;
  };

  inline Matrix44 view_transformation(Point from, Point to, Vector up)
  {
    auto forward = (to - from).normalize();
    auto up_normal = up.normalize();
    auto left = forward.crossProduct(up_normal);
    auto true_up = left.crossProduct(forward);
    // clang-format off
    auto orientation = Matrix44{{    left.x(),     left.y(),     left.z(), 0,
                                  true_up.x(),  true_up.y(),  true_up.z(), 0,
                                 -forward.x(), -forward.y(), -forward.z(), 0,
                                            0,            0,            0, 1 }};
    // clang-format on
    return orientation * Matrix44::translation(-from.x(), -from.y(), -from.z());
  }
}
