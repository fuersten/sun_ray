//
//  cone.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 26.04.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/object.h>


namespace sunray
{
  class Cone;
  using ConePtr = std::shared_ptr<const Cone>;

  class Cone
  : public Object
  , public std::enable_shared_from_this<Cone>
  {
  public:
    Cone(double maximum = std::numeric_limits<double>::infinity(), double minimum = -std::numeric_limits<double>::infinity(),
         bool closed = false)
    : maximum_{maximum}
    , minimum_{minimum}
    , closed_{closed}
    {
    }

    explicit Cone(Material material, double maximum = std::numeric_limits<double>::infinity(),
                  double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    : Object(std::move(material))
    , maximum_{maximum}
    , minimum_{minimum}
    , closed_{closed}
    {
    }

    explicit Cone(Matrix44 transformation, double maximum = std::numeric_limits<double>::infinity(),
                  double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    : Object(std::move(transformation))
    , maximum_{maximum}
    , minimum_{minimum}
    , closed_{closed}
    {
    }

    Cone(Material material, Matrix44 transformation, bool casts_shadow = true,
         double maximum = std::numeric_limits<double>::infinity(), double minimum = -std::numeric_limits<double>::infinity(),
         bool closed = false)
    : Object(std::move(material), std::move(transformation), casts_shadow)
    , maximum_{maximum}
    , minimum_{minimum}
    , closed_{closed}
    {
    }

    ~Cone() override = default;

    Cone(const Cone&) = delete;
    Cone(Cone&&) = delete;
    Cone& operator=(const Cone&) = delete;
    Cone& operator=(Cone&&) = delete;

    static ConePtr make_cone(double maximum = std::numeric_limits<double>::infinity(),
                             double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    {
      return std::make_shared<Cone>(maximum, minimum, closed);
    }

    static ConePtr make_cone(Material material, double maximum = std::numeric_limits<double>::infinity(),
                             double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    {
      return std::make_shared<Cone>(std::move(material), maximum, minimum, closed);
    }

    static ConePtr make_cone(const Matrix44& transformation, double maximum = std::numeric_limits<double>::infinity(),
                             double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    {
      return std::make_shared<Cone>(transformation, maximum, minimum, closed);
    }

    static ConePtr make_cone(Material material, const Matrix44& transformation, bool casts_shadow = true,
                             double maximum = std::numeric_limits<double>::infinity(),
                             double minimum = -std::numeric_limits<double>::infinity(), bool closed = false)
    {
      return std::make_shared<Cone>(std::move(material), transformation, casts_shadow, maximum, minimum, closed);
    }

  private:
    bool do_intersected_by(const Ray& ray, Intersections& intersections) const override
    {
      const auto a = pow<2>(ray.direction().x()) - pow<2>(ray.direction().y()) + pow<2>(ray.direction().z());

      const auto b = 2 * ray.origin().x() * ray.direction().x() - 2 * ray.origin().y() * ray.direction().y() +
                     2 * ray.origin().z() * ray.direction().z();
      const auto c = pow<2>(ray.origin().x()) - pow<2>(ray.origin().y()) + pow<2>(ray.origin().z());

      bool result{false};

      if (abs(a) < epsilon && abs(b) >= epsilon) {
        auto t = -c / (2 * b);
        intersections.add(Intersection{t, this});
        result = true;
      }

      if (abs(a) >= epsilon) {
        const auto discriminant = pow<2>(b) - 4 * a * c;

        if (discriminant >= 0) {
          auto sr = sqrt(discriminant);
          auto a2 = 2 * a;
          auto t0 = (-b - sr) / a2;
          auto t1 = (-b + sr) / a2;

          if (t0 > t1) {
            std::swap(t0, t1);
          }

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
      }
      result |= intersect_caps(ray, intersections);

      return result;
    }

    inline bool check_caps(const Ray& ray, double t, double h) const
    {
      const auto x = ray.origin().x() + t * ray.direction().x();
      const auto z = ray.origin().z() + t * ray.direction().z();

      return (pow<2>(x) + pow<2>(z)) <= pow<2>(h);
    }

    bool intersect_caps(const Ray& ray, Intersections& intersections) const
    {
      if (!closed_ || abs(ray.direction().y()) <= epsilon) {
        return false;
      }

      bool result{false};

      auto t = (minimum_ - ray.origin().y()) / ray.direction().y();
      if (check_caps(ray, t, minimum_)) {
        intersections.add(Intersection{t, this});
        result = true;
      }

      t = (maximum_ - ray.origin().y()) / ray.direction().y();
      if (check_caps(ray, t, maximum_)) {
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

      auto y = sqrt(distance);
      if (local_point.y() > 0) {
        y *= -1;
      }

      return create_vector(local_point.x(), y, local_point.z());
    }

    double maximum_{std::numeric_limits<double>::infinity()};
    double minimum_{-std::numeric_limits<double>::infinity()};
    bool closed_{false};
  };
}
