//
//  triangle.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 09.07.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/object.h>


namespace sunray
{
  class Triangle;
  using TrianglePtr = std::shared_ptr<const Triangle>;

  class Triangle
  : public Object
  , public std::enable_shared_from_this<Triangle>
  {
  public:
    explicit Triangle(Point p1, Point p2, Point p3)
    : Object()
    , p1_{p1}
    , p2_{p2}
    , p3_{p3}
    , e1_{p2_ - p1_}
    , e2_{p3_ - p1_}
    {
      normal_ = e2_.crossProduct(e1_).normalize();
    }

    explicit Triangle(Material material, Point p1, Point p2, Point p3)
    : Object(std::move(material))
    , p1_{p1}
    , p2_{p2}
    , p3_{p3}
    , e1_{p2_ - p1_}
    , e2_{p3_ - p1_}
    {
      normal_ = e2_.crossProduct(e1_).normalize();
    }

    explicit Triangle(Matrix44 transformation, Point p1, Point p2, Point p3)
    : Object(std::move(transformation))
    , p1_{p1}
    , p2_{p2}
    , p3_{p3}
    , e1_{p2_ - p1_}
    , e2_{p3_ - p1_}
    {
      normal_ = e2_.crossProduct(e1_).normalize();
    }

    Triangle(Material material, Matrix44 transformation, Point p1, Point p2, Point p3, bool casts_shadow = true)
    : Object(std::move(material), std::move(transformation), casts_shadow)
    , p1_{p1}
    , p2_{p2}
    , p3_{p3}
    , e1_{p2_ - p1_}
    , e2_{p3_ - p1_}
    {
      normal_ = e2_.crossProduct(e1_).normalize();
    }

    ~Triangle() override = default;

    Triangle(const Triangle&) = delete;
    Triangle(Triangle&&) = delete;
    Triangle& operator=(const Triangle&) = delete;
    Triangle& operator=(Triangle&&) = delete;

    static TrianglePtr make_triangle(Point p1, Point p2, Point p3)
    {
      return std::make_shared<Triangle>(p1, p2, p3);
    }

    static TrianglePtr make_triangle(Material material, Point p1, Point p2, Point p3)
    {
      return std::make_shared<Triangle>(std::move(material), p1, p2, p3);
    }

    static TrianglePtr make_triangle(const Matrix44& transformation, Point p1, Point p2, Point p3)
    {
      return std::make_shared<Triangle>(transformation, p1, p2, p3);
    }

    static TrianglePtr make_triangle(Material material, const Matrix44& transformation, Point p1, Point p2, Point p3,
                                     bool casts_shadow = true)
    {
      return std::make_shared<Triangle>(std::move(material), transformation, p1, p2, p3, casts_shadow);
    }

    const Vector& p1() const
    {
      return p1_;
    }

    const Vector& p2() const
    {
      return p2_;
    }

    const Vector& p3() const
    {
      return p3_;
    }

    const Vector& e1() const
    {
      return e1_;
    }

    const Vector& e2() const
    {
      return e2_;
    }

  private:
    bool do_intersected_by(const Ray& ray, Intersections& intersections) const override
    {
      const auto dir_cross_e2 = ray.direction().crossProduct(e2_);
      const auto det = e1_.scalarProduct(dir_cross_e2);
      if (abs(det) < epsilon) {
        return false;
      }

      const auto f = 1.0f / det;

      const auto p1_to_origin = ray.origin() - p1_;
      const auto u = f * p1_to_origin.scalarProduct(dir_cross_e2);
      if (u < 0 || u > 1) {
        return false;
      }

      const auto origin_cross_e1 = p1_to_origin.crossProduct(e1_);
      const auto v = f * ray.direction().scalarProduct(origin_cross_e1);
      if (v < 0 || (u + v) > 1) {
        return false;
      }

      const auto t = f * e2_.scalarProduct(origin_cross_e1);

      intersections.add(Intersection{t, this});
      return true;
    }

    Vector do_normal_at(const Point& local_point) const override
    {
      (void)local_point;
      return normal_;
    }

    Point p1_;
    Point p2_;
    Point p3_;
    Vector normal_;
    Vector e1_;
    Vector e2_;
  };
}
