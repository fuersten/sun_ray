//
//  disk.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 19.04.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/shape.h>


namespace sunray
{
  class Disk;
  using DiskPtr = std::shared_ptr<const Disk>;

  class Disk
  : public Shape
  , public std::enable_shared_from_this<Disk>
  {
  public:
    explicit Disk(double inner_radius)
    : Shape()
    , inner_radius_{inner_radius}
    {
    }

    explicit Disk(Material material, double inner_radius = 0.0)
    : Shape(std::move(material))
    , inner_radius_{inner_radius}
    {
    }

    explicit Disk(Matrix44 transformation, double inner_radius = 0.0)
    : Shape(std::move(transformation))
    , inner_radius_{inner_radius}
    {
    }

    Disk(Material material, Matrix44 transformation, bool casts_shadow = true, double inner_radius = 0.0)
    : Shape(std::move(material), std::move(transformation), casts_shadow)
    , inner_radius_{inner_radius}
    {
    }

    ~Disk() override = default;

    Disk(const Disk&) = delete;
    Disk(Disk&&) = delete;
    Disk& operator=(const Disk&) = delete;
    Disk& operator=(Disk&&) = delete;

    static DiskPtr make_disk(double inner_radius = 0.0)
    {
      return std::make_shared<Disk>(inner_radius);
    }

    static DiskPtr make_disk(Material material, double inner_radius = 0.0)
    {
      return std::make_shared<Disk>(std::move(material), inner_radius);
    }

    static DiskPtr make_disk(const Matrix44& transformation, double inner_radius = 0.0)
    {
      return std::make_shared<Disk>(transformation, inner_radius);
    }

    static DiskPtr make_disk(Material material, const Matrix44& transformation, bool casts_shadow = true,
                             double inner_radius = 0.0)
    {
      return std::make_shared<Disk>(std::move(material), transformation, casts_shadow, inner_radius);
    }

  private:
    bool do_intersected_by(const Ray& ray, Intersections& intersections) const override
    {
      if (abs(ray.direction().y()) < epsilon) {
        return false;
      }

      const auto t = -ray.origin().y() / ray.direction().y();
      const auto p = ray.point_at(t);
      const double dist = create_vector(p.x(), p.y(), p.z()).magnitude();

      if (dist > radius_ * radius_ || dist < inner_radius_ * inner_radius_) {
        return false;
      }

      intersections.add(Intersection{t, this});

      return true;
    }

    Vector do_normal_at(const Point& local_point) const override
    {
      (void)local_point;
      return create_vector(0, 1, 0);
    }

    double radius_{1.0};
    double inner_radius_{0.0};
  };
}
