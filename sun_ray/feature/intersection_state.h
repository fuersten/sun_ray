//
//  intersection_state.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 18.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/object.h>


namespace sunray
{
  class IntersectionState
  {
  public:
    IntersectionState(Intersection intersection, const Ray& ray, const Intersections& intersections)
    : intersection_{std::move(intersection)}
    {
      point_ = ray.position(intersection_.time());
      eye_ = -ray.direction();
      normal_ = intersection_.object()->normal_at(point_);
      if (normal_.scalarProduct(eye_) < 0) {
        inside_ = true;
        normal_ = normal_.negate();
      }
      over_point_ = point_ + normal_ * epsilon;
      under_point_ = point_ - normal_ * epsilon;
      reflect_ = ray.direction().reflect(normal_);

      calculate_refraction_indices(intersections);
    }

    ~IntersectionState() = default;

    IntersectionState(const IntersectionState&) = delete;
    IntersectionState(IntersectionState&&) = delete;
    IntersectionState& operator=(const IntersectionState&) = delete;
    IntersectionState& operator=(IntersectionState&&) = delete;

    const Intersection& intersection() const
    {
      return intersection_;
    }

    inline const Point& point() const
    {
      return point_;
    }

    inline const Point& over_point() const
    {
      return over_point_;
    }

    inline const Point& under_point() const
    {
      return under_point_;
    }

    inline const Vector& eye() const
    {
      return eye_;
    }

    inline const Vector& normal() const
    {
      return normal_;
    }

    inline const Vector& reflect() const
    {
      return reflect_;
    }

    inline bool is_inside() const
    {
      return inside_;
    }

    inline float n1() const
    {
      return n1_;
    }

    inline float n2() const
    {
      return n2_;
    }

    float schlick() const
    {
      auto cos = eye_.scalarProduct(normal_);

      if (n1_ > n2_) {
        const auto n = n1_ / n2_;
        const auto sin2_t = n * n * (1.0 - cos * cos);
        if (sin2_t > 1.0) {
          return 1.0;
        }

        cos = sqrt(1.0 - sin2_t);
      }

      const auto r0 = pow<2>((n1_ - n2_) / (n1_ + n2_));
      return static_cast<float>(r0 + (1.0 - r0) * pow<5>(1.0 - cos));
    }

  private:
    void calculate_refraction_indices(const Intersections& intersections)
    {
      std::vector<const Object*> objects;
      objects.reserve(20);

      for (const auto& intersection : intersections.intersections()) {
        if (intersection == intersection_) {
          if (objects.empty()) {
            n1_ = 1.0f;
          } else {
            n1_ = objects.back()->material().refractive_index();
          }
        }

        auto it = std::find(objects.begin(), objects.end(), intersection.object());
        if (it != objects.end()) {
          objects.erase(it);
        } else {
          objects.emplace_back(intersection.object());
        }

        if (intersection == intersection_) {
          if (objects.empty()) {
            n2_ = 1.0;
          } else {
            n2_ = objects.back()->material().refractive_index();
          }

          break;
        }
      }
    }

    Intersection intersection_;
    Point point_;
    Point over_point_;
    Point under_point_;
    Vector eye_;
    Vector normal_;
    Vector reflect_;
    bool inside_{false};
    float n1_{1.0};
    float n2_{1.0};
  };
}
