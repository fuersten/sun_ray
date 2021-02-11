//
//  object.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 05.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/intersection.h>
#include <sun_ray/feature/material.h>
#include <sun_ray/feature/ray.h>


namespace sunray
{
  class Object
  {
  public:
    virtual ~Object() = default;

    Object(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(const Object&) = delete;
    Object& operator=(Object&&) = delete;

    inline const auto& inverse_transformation() const
    {
      return inverse_transformation_;
    }

    bool is_intersected_by(const Ray& ray, Intersections& intersections) const
    {
      return do_intersected_by(ray.transform(inverse_transformation()), intersections);
    }

    Vector normal_at(const Point& world_point) const
    {
      const auto& m = inverse_transformation();
      const auto local_point = m * world_point;
      const auto local_normal = do_normal_at(local_point);
      const auto world_normal = m.transpose() * local_normal;
      return Tuple(world_normal.x(), world_normal.y(), world_normal.z(), 0.0).normalize();
    }

    inline const Point& origin() const
    {
      return origin_;
    }

    inline const Matrix44& transformation() const
    {
      return transformation_;
    }

    inline const Material& material() const
    {
      return material_;
    }

    inline bool casts_shadow() const
    {
      return casts_shadow_;
    }

  protected:
    Object() = default;

    Object(Material material)
    : material_{std::move(material)}
    {
    }

    Object(Matrix44 transformation)
    : transformation_{std::move(transformation)}
    , inverse_transformation_{transformation_.inverse()}
    {
    }

    Object(Material material, Matrix44 transformation, bool casts_shadow = true)
    : material_{std::move(material)}
    , transformation_{std::move(transformation)}
    , inverse_transformation_{transformation_.inverse()}
    , casts_shadow_{casts_shadow}
    {
    }

  private:
    virtual bool do_intersected_by(const Ray& ray, Intersections& intersections) const = 0;

    virtual Vector do_normal_at(const Point& point) const = 0;

    Point origin_{create_point(0, 0, 0)};
    Material material_;
    const Matrix44 transformation_{Matrix44::identity()};
    const Matrix44 inverse_transformation_{Matrix44::identity().inverse()};
    bool casts_shadow_{true};
  };

  inline Intersections intersect(const Ray& ray, const Object& object)
  {
    Intersections intersections;
    object.is_intersected_by(ray, intersections);
    return intersections;
  }

  inline Color apply_pattern(const Pattern& pattern, const Object& object, const Point& position)
  {
    auto object_point = object.inverse_transformation() * position;
    auto pattern_point = pattern.inverse_transformation() * object_point;
    return pattern.pattern_at(pattern_point);
  }

  inline Color lighting(const Object& object, const PointLight& light, const Point& position, const Vector& eye,
                        const Vector& normal, bool is_in_shadow)
  {
    Color color{object.material().color()};
    if (object.material().pattern()) {
      color = apply_pattern(*object.material().pattern(), object, position);
    }
    const auto effective_color = color * light.intensity();
    const auto lightv = (light.position() - position).normalize();
    const auto effective_ambient = effective_color * object.material().ambient();
    const auto light_dot_normal = static_cast<float>(lightv.scalarProduct(normal));
    if (light_dot_normal < 0.0 || is_in_shadow) {
      return effective_ambient;
    }
    const auto effective_diffuse = effective_color * object.material().diffuse() * light_dot_normal;
    const auto reflectv = lightv.negate().reflect(normal);
    const auto reflect_dot_eye = static_cast<float>(reflectv.scalarProduct(eye));
    if (reflect_dot_eye <= 0.0) {
      return effective_ambient + effective_diffuse;
    }
    const auto factor = powf(reflect_dot_eye, object.material().shininess());
    const auto effective_specular = light.intensity() * object.material().specular() * factor;
    return effective_ambient + effective_diffuse + effective_specular;
  }
}
