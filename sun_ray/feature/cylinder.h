//
//  cylinder.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 25.04.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/object.h>


namespace sunray
{
  class Cylinder;
  using CylinderPtr = std::shared_ptr<const Cylinder>;

  class Cylinder
  : public Object
  , public std::enable_shared_from_this<Cylinder>
  {
  public:
    Cylinder(double maximum = std::numeric_limits<double>::infinity(), double minimum = -std::numeric_limits<double>::infinity())
    : maximum_{maximum}
    , minimum_{minimum}
    {
    }

    explicit Cylinder(Material material, double maximum = std::numeric_limits<double>::infinity(),
                      double minimum = -std::numeric_limits<double>::infinity())
    : Object(std::move(material))
    , maximum_{maximum}
    , minimum_{minimum}
    {
    }

    explicit Cylinder(Matrix44 transformation, double maximum = std::numeric_limits<double>::infinity(),
                      double minimum = -std::numeric_limits<double>::infinity())
    : Object(std::move(transformation))
    , maximum_{maximum}
    , minimum_{minimum}
    {
    }

    Cylinder(Material material, Matrix44 transformation, bool casts_shadow = true,
             double maximum = std::numeric_limits<double>::infinity(), double minimum = -std::numeric_limits<double>::infinity())
    : Object(std::move(material), std::move(transformation), casts_shadow)
    , maximum_{maximum}
    , minimum_{minimum}
    {
    }

    ~Cylinder() override = default;

    Cylinder(const Cylinder&) = delete;
    Cylinder(Cylinder&&) = delete;
    Cylinder& operator=(const Cylinder&) = delete;
    Cylinder& operator=(Cylinder&&) = delete;

    static CylinderPtr make_cylinder(double maximum = std::numeric_limits<double>::infinity(),
                                     double minimum = -std::numeric_limits<double>::infinity())
    {
      return std::make_shared<Cylinder>(maximum, minimum);
    }

    static CylinderPtr make_cylinder(Material material, double maximum = std::numeric_limits<double>::infinity(),
                                     double minimum = -std::numeric_limits<double>::infinity())
    {
      return std::make_shared<Cylinder>(std::move(material), maximum, minimum);
    }

    static CylinderPtr make_cylinder(const Matrix44& transformation, double maximum = std::numeric_limits<double>::infinity(),
                                     double minimum = -std::numeric_limits<double>::infinity())
    {
      return std::make_shared<Cylinder>(transformation, maximum, minimum);
    }

    static CylinderPtr make_cylinder(Material material, const Matrix44& transformation, bool casts_shadow = true,
                                     double maximum = std::numeric_limits<double>::infinity(),
                                     double minimum = -std::numeric_limits<double>::infinity())
    {
      return std::make_shared<Cylinder>(std::move(material), transformation, casts_shadow, maximum, minimum);
    }

  private:
    bool do_intersected_by(const Ray& ray, Intersections& intersections) const override
    {
      const auto a = pow<2>(ray.direction().x()) + pow<2>(ray.direction().z());

      if (::fabs(a) < epsilon) {
        return false;
      }

      const auto b = 2 * ray.origin().x() * ray.direction().x() + 2 * ray.origin().z() * ray.direction().z();
      const auto c = pow<2>(ray.origin().x()) + pow<2>(ray.origin().z()) - 1;

      const auto discriminant = pow<2>(b) - 4 * a * c;

      if (discriminant < 0) {
        return false;
      }

      auto t0 = (-b - sqrt(discriminant)) / (2 * a);
      auto t1 = (-b + sqrt(discriminant)) / (2 * a);

      if (t0 > t1) {
        std::swap(t0, t1);
      }

      bool result{false};
      auto y0 = ray.origin().y() + t0 * ray.direction().y();
      if (minimum_ < y0 && y0 < maximum_) {
        intersections.add(Intersection{t0, this});
        result = true;
      }

      auto y1 = ray.origin().y() + t1 * ray.direction().y();
      if (minimum_ < y1 && y1 < maximum_) {
        intersections.add(Intersection{t1, this});
        result = true;
      }

      return result;
    }

    Vector do_normal_at(const Point& local_point) const override
    {
      return create_vector(local_point.x(), 0, local_point.z());
    }

    double maximum_{std::numeric_limits<double>::infinity()};
    double minimum_{-std::numeric_limits<double>::infinity()};
  };
}
