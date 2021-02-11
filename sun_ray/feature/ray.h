//
//  ray.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 04.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/matrix.h>
#include <sun_ray/feature/tuple.h>


namespace sunray
{
  class Ray
  {
  public:
    Ray(Point origin, Vector direction)
    : origin_{std::move(origin)}
    , direction_{std::move(direction)}
    {
    }

    ~Ray() = default;

    Ray(const Ray&) = delete;
    Ray(Ray&&) = delete;
    Ray& operator=(const Ray&) = delete;
    Ray& operator=(Ray&&) = delete;

    Point point_at(double t) const
    {
      return origin_ + direction_ * t;
    }

    friend bool operator==(const Ray& lhs, const Ray& rhs)
    {
      return lhs.origin_ == rhs.origin_ && lhs.direction_ == rhs.direction_;
    }

    inline const Point& origin() const
    {
      return origin_;
    }

    inline const Vector& direction() const
    {
      return direction_;
    }

    Point position(double t) const
    {
      return origin_ + direction_ * t;
    }

    Ray transform(const Matrix44& matrix) const
    {
      return Ray{matrix * origin(), matrix * direction()};
    }

    friend std::ostream& operator<<(std::ostream& stream, const Ray& ray)
    {
      stream << "origin: " << ray.origin() << " direction: " << ray.direction();
      return stream;
    }

  private:
    Point origin_;
    Vector direction_;
  };
}
