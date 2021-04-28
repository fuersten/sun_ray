//
//  cylinder.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 25.04.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/shape.h>


namespace sunray
{
  class Cylinder;
  using CylinderPtr = std::shared_ptr<const Cylinder>;

  class Cylinder
  : public Shape
  , public std::enable_shared_from_this<Cylinder>
  {
  public:
    Cylinder(double maximum = std::numeric_limits<double>::infinity(), double minimum = -std::numeric_limits<double>::infinity(),
             bool closed = false)
    : maximum_{maximum}
    , minimum_{minimum}
    , closed_{closed}
    {
    }

    explicit Cylinder(Material material, double maximum = std::numeric_limits<double>::infinity(),
                      double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    : Shape(std::move(material))
    , maximum_{maximum}
    , minimum_{minimum}
    , closed_{closed}
    {
    }

    explicit Cylinder(Matrix44 transformation, double maximum = std::numeric_limits<double>::infinity(),
                      double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    : Shape(std::move(transformation))
    , maximum_{maximum}
    , minimum_{minimum}
    , closed_{closed}
    {
    }

    Cylinder(Material material, Matrix44 transformation, bool casts_shadow = true,
             double maximum = std::numeric_limits<double>::infinity(), double minimum = -std::numeric_limits<double>::infinity(),
             bool closed = false)
    : Shape(std::move(material), std::move(transformation), casts_shadow)
    , maximum_{maximum}
    , minimum_{minimum}
    , closed_{closed}
    {
    }

    ~Cylinder() override = default;

    Cylinder(const Cylinder&) = delete;
    Cylinder(Cylinder&&) = delete;
    Cylinder& operator=(const Cylinder&) = delete;
    Cylinder& operator=(Cylinder&&) = delete;

    static CylinderPtr make_cylinder(double maximum = std::numeric_limits<double>::infinity(),
                                     double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    {
      return std::make_shared<Cylinder>(maximum, minimum, closed);
    }

    static CylinderPtr make_cylinder(Material material, double maximum = std::numeric_limits<double>::infinity(),
                                     double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    {
      return std::make_shared<Cylinder>(std::move(material), maximum, minimum, closed);
    }

    static CylinderPtr make_cylinder(const Matrix44& transformation, double maximum = std::numeric_limits<double>::infinity(),
                                     double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    {
      return std::make_shared<Cylinder>(transformation, maximum, minimum, closed);
    }

    static CylinderPtr make_cylinder(Material material, const Matrix44& transformation, bool casts_shadow = true,
                                     double maximum = std::numeric_limits<double>::infinity(),
                                     double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    {
      return std::make_shared<Cylinder>(std::move(material), transformation, casts_shadow, maximum, minimum, closed);
    }

  private:
    bool do_intersected_by(const Ray& ray, Intersections& intersections) const override
    {
      const auto a = pow<2>(ray.direction().x()) + pow<2>(ray.direction().z());

      if (!closed_ && abs(a) < epsilon) {
        return false;
      }

      const auto b = 2 * ray.origin().x() * ray.direction().x() + 2 * ray.origin().z() * ray.direction().z();
      const auto c = pow<2>(ray.origin().x()) + pow<2>(ray.origin().z()) - 1;

      const auto discriminant = pow<2>(b) - 4 * a * c;

      if (discriminant < 0) {
        return false;
      }

      auto sr = sqrt(discriminant);
      auto a2 = 2 * a;
      auto t0 = (-b - sr) / a2;
      auto t1 = (-b + sr) / a2;

      if (t0 > t1) {
        std::swap(t0, t1);
      }

      bool result{false};

      if (abs(a) >= epsilon) {
        const auto y0 = ray.origin().y() + t0 * ray.direction().y();
        if (minimum_ < y0 && y0 < maximum_) {
          intersections.add(Intersection{t0, this});
          result = true;
        }

        const auto y1 = ray.origin().y() + t1 * ray.direction().y();
        if (minimum_ < y1 && y1 < maximum_) {
          intersections.add(Intersection{t1, this});
          result = true;
        }
      }
      result |= intersect_caps(ray, intersections);

      return result;
    }

    inline bool check_caps(const Ray& ray, double t) const
    {
      const auto x = ray.origin().x() + t * ray.direction().x();
      const auto z = ray.origin().z() + t * ray.direction().z();

      return (pow<2>(x) + pow<2>(z)) <= 1;
    }

    bool intersect_caps(const Ray& ray, Intersections& intersections) const
    {
      if (!closed_ || abs(ray.direction().y()) <= epsilon) {
        return false;
      }

      bool result{false};

      auto t = (minimum_ - ray.origin().y()) / ray.direction().y();
      if (check_caps(ray, t)) {
        intersections.add(Intersection{t, this});
        result = true;
      }

      t = (maximum_ - ray.origin().y()) / ray.direction().y();
      if (check_caps(ray, t)) {
        intersections.add(Intersection{t, this});
        result = true;
      }

      return result;
    }

    Vector do_normal_at(const Point& local_point) const override
    {
      const auto distance = pow<2>(local_point.x()) + pow<2>(local_point.z());

      if (distance < 1 && local_point.y() >= (maximum_ - epsilon)) {
        return create_vector(0, 1, 0);
      } else if (distance < 1 && local_point.y() <= (minimum_ + epsilon)) {
        return create_vector(0, -1, 0);
      }

      return create_vector(local_point.x(), 0, local_point.z());
    }

    double maximum_{std::numeric_limits<double>::infinity()};
    double minimum_{-std::numeric_limits<double>::infinity()};
    bool closed_{false};
  };
}
