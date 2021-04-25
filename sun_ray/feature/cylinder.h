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
    Cylinder() = default;

    explicit Cylinder(Material material)
    : Object(std::move(material))
    {
    }

    explicit Cylinder(Matrix44 transformation)
    : Object(std::move(transformation))
    {
    }

    Cylinder(Material material, Matrix44 transformation, bool casts_shadow = true)
    : Object(std::move(material), std::move(transformation), casts_shadow)
    {
    }

    ~Cylinder() override = default;

    Cylinder(const Cylinder&) = delete;
    Cylinder(Cylinder&&) = delete;
    Cylinder& operator=(const Cylinder&) = delete;
    Cylinder& operator=(Cylinder&&) = delete;

    static CylinderPtr make_cylinder()
    {
      return std::make_shared<Cylinder>();
    }

    static CylinderPtr make_cylinder(Material material)
    {
      return std::make_shared<Cylinder>(std::move(material));
    }

    static CylinderPtr make_cylinder(const Matrix44& transformation)
    {
      return std::make_shared<Cylinder>(transformation);
    }

    static CylinderPtr make_cylinder(Material material, const Matrix44& transformation, bool casts_shadow = true)
    {
      return std::make_shared<Cylinder>(std::move(material), transformation, casts_shadow);
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

      Intersection i1{(-b - sqrt(discriminant)) / (2 * a), this};
      Intersection i2{(-b + sqrt(discriminant)) / (2 * a), this};
      intersections.add(std::move(i1));
      intersections.add(std::move(i2));

      return true;
    }

    Vector do_normal_at(const Point& local_point) const override
    {
      return create_vector(local_point.x(), 0, local_point.z());
    }
  };
}
