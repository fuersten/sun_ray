//
//  plane.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 04.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/object.h>


namespace sunray
{
  class Plane;
  using PlanePtr = std::shared_ptr<const Plane>;

  class Plane
  : public Object
  , public std::enable_shared_from_this<Plane>
  {
  public:
    Plane() = default;

    explicit Plane(Material material)
    : Object(std::move(material))
    {
    }

    explicit Plane(Matrix44 transformation)
    : Object(std::move(transformation))
    {
    }

    Plane(Material material, Matrix44 transformation, bool casts_shadow = true)
    : Object(std::move(material), std::move(transformation), casts_shadow)
    {
    }

    ~Plane() override = default;

    Plane(const Plane&) = delete;
    Plane(Plane&&) = delete;
    Plane& operator=(const Plane&) = delete;
    Plane& operator=(Plane&&) = delete;

    static PlanePtr make_plane()
    {
      return std::make_shared<Plane>();
    }

    static PlanePtr make_plane(Material material)
    {
      return std::make_shared<Plane>(std::move(material));
    }

    static PlanePtr make_plane(const Matrix44& transformation)
    {
      return std::make_shared<Plane>(transformation);
    }

    static PlanePtr make_plane(Material material, const Matrix44& transformation, bool casts_shadow = true)
    {
      return std::make_shared<Plane>(std::move(material), transformation, casts_shadow);
    }

  private:
    bool do_intersected_by(const Ray& ray, Intersections& intersections) const override
    {
      if (abs(ray.direction().y()) < epsilon) {
        return false;
      }

      intersections.add(Intersection{-ray.origin().y() / ray.direction().y(), this});

      return true;
    }

    Vector do_normal_at(const Point& local_point) const override
    {
      (void)local_point;
      return create_vector(0, 1, 0);
    }
  };
}
