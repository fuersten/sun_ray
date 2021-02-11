//
//  sphere.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 05.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/object.h>


namespace sunray
{
  class Sphere;
  using SpherePtr = std::shared_ptr<const Sphere>;

  class Sphere
  : public Object
  , public std::enable_shared_from_this<Sphere>
  {
  public:
    Sphere() = default;

    explicit Sphere(Material material)
    : Object(std::move(material))
    {
    }

    explicit Sphere(Matrix44 transformation)
    : Object(std::move(transformation))
    {
    }

    Sphere(Material material, Matrix44 transformation, bool casts_shadow = true)
    : Object(std::move(material), std::move(transformation), casts_shadow)
    {
    }

    ~Sphere() override = default;

    Sphere(const Sphere&) = delete;
    Sphere(Sphere&&) = delete;
    Sphere& operator=(const Sphere&) = delete;
    Sphere& operator=(Sphere&&) = delete;

    static SpherePtr make_sphere()
    {
      return std::make_shared<Sphere>();
    }

    static SpherePtr make_sphere(Material material)
    {
      return std::make_shared<Sphere>(std::move(material));
    }

    static SpherePtr make_sphere(const Matrix44& transformation)
    {
      return std::make_shared<Sphere>(transformation);
    }

    static SpherePtr make_sphere(Material material, const Matrix44& transformation, bool casts_shadow = true)
    {
      return std::make_shared<Sphere>(std::move(material), transformation, casts_shadow);
    }

  private:
    bool do_intersected_by(const Ray& ray, Intersections& intersections) const override
    {
      const auto sphere_to_ray = ray.origin() - origin();
      const auto a = ray.direction().scalarProduct(ray.direction());
      const auto b = 2 * ray.direction().scalarProduct(sphere_to_ray);
      const auto c = sphere_to_ray.scalarProduct(sphere_to_ray) - 1;
      const auto discriminant = b * b - 4 * a * c;

      if (discriminant < 0.0) {
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
      return local_point - origin();
    }
  };
}
