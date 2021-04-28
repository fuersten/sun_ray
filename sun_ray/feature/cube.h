//
//  cube.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 13.04.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/shape.h>


namespace sunray
{
  class Cube;
  using CubePtr = std::shared_ptr<const Cube>;

  class Cube
  : public Shape
  , public std::enable_shared_from_this<Cube>
  {
  public:
    Cube() = default;

    explicit Cube(Material material)
    : Shape(std::move(material))
    {
    }

    explicit Cube(Matrix44 transformation)
    : Shape(std::move(transformation))
    {
    }

    Cube(Material material, Matrix44 transformation, bool casts_shadow = true)
    : Shape(std::move(material), std::move(transformation), casts_shadow)
    {
    }

    ~Cube() override = default;

    Cube(const Cube&) = delete;
    Cube(Cube&&) = delete;
    Cube& operator=(const Cube&) = delete;
    Cube& operator=(Cube&&) = delete;

    static CubePtr make_cube()
    {
      return std::make_shared<Cube>();
    }

    static CubePtr make_cube(Material material)
    {
      return std::make_shared<Cube>(std::move(material));
    }

    static CubePtr make_cube(const Matrix44& transformation)
    {
      return std::make_shared<Cube>(transformation);
    }

    static CubePtr make_cube(Material material, const Matrix44& transformation, bool casts_shadow = true)
    {
      return std::make_shared<Cube>(std::move(material), transformation, casts_shadow);
    }

  private:
    bool do_intersected_by(const Ray& ray, Intersections& intersections) const override
    {
      auto x = check_axis(ray.origin().x(), ray.direction().x());
      auto y = check_axis(ray.origin().y(), ray.direction().y());
      auto z = check_axis(ray.origin().z(), ray.direction().z());

      auto tmin = std::max(std::max(x.first, y.first), z.first);
      auto tmax = std::min(std::min(x.second, y.second), z.second);

      if (tmin > tmax) {
        return false;
      }

      intersections.add(Intersection{tmin, this});
      intersections.add(Intersection{tmax, this});

      return true;
    }

    std::pair<double, double> check_axis(double origin, double direction) const
    {
      auto tmin_num = -1 - origin;
      auto tmax_num = 1 - origin;

      double tmin = 0.0;
      double tmax = 0.0;

      if (abs(direction) >= epsilon) {
        tmin = tmin_num / direction;
        tmax = tmax_num / direction;
      } else {
        tmin = tmin_num * INFINITY;
        tmax = tmax_num * INFINITY;
      }

      if (tmin > tmax) {
        std::swap(tmin, tmax);
      }

      return std::make_pair(tmin, tmax);
    }

    Vector do_normal_at(const Point& local_point) const override
    {
      auto absx = abs(local_point.x());
      auto absy = abs(local_point.y());
      auto maxc = std::max(std::max(absx, absy), abs(local_point.z()));

      if (Approx(maxc) == absx) {
        return create_vector(local_point.x(), 0, 0);
      } else if (Approx(maxc) == absy) {
        return create_vector(0, local_point.y(), 0);
      }

      return create_vector(0, 0, local_point.z());
    }
  };
}
