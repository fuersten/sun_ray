//
//  shape.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 03.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/object.h>
#include <sun_ray/feature/transformation.h>
#include <sun_ray/script/class.h>
#include <sun_ray/script/meta_class.h>
#include <sun_ray/script/objects/material.h>


namespace sunray
{
  namespace script
  {
    class Shape
    : public Class
    , public std::enable_shared_from_this<Shape>
    {
    public:
      Shape(MetaClassPtr meta_class, const Material& material)
      : Class(meta_class)
      , material_{material.material()}
      {
        trans_.identity();
      }

      MutableClassPtr scale(double x, double y, double z)
      {
        trans_.scale(x, y, z);
        return shared_from_this();
      }

      MutableClassPtr rotate_x(double radians)
      {
        trans_.rotate_x(radians);
        return shared_from_this();
      }

      MutableClassPtr rotate_y(double radians)
      {
        trans_.rotate_y(radians);
        return shared_from_this();
      }

      MutableClassPtr rotate_z(double radians)
      {
        trans_.rotate_z(radians);
        return shared_from_this();
      }

      MutableClassPtr translate(double x, double y, double z)
      {
        trans_.translate(x, y, z);
        return shared_from_this();
      }

      MutableClassPtr shear(double xy, double xz, double yx, double yz, double zx, double zy)
      {
        trans_.shear(xy, xz, yx, yz, zx, zy);
        return shared_from_this();
      }

      Matrix44 transformation() const
      {
        return trans_.matrix();
      }

      void casts_shadow(bool value)
      {
        casts_shadow_ = value;
      }

      virtual std::shared_ptr<const sunray::Object> shape() const = 0;

    protected:
      mutable sunray::Transformation trans_;
      sunray::Material material_;
      bool casts_shadow_{true};
    };


    class ShapeMetaClass : public sunray::script::MetaClass<Shape>
    {
    protected:
      static MutableClassPtr scale(sunray::script::MutableClassPtr& c, double x, double y, double z)
      {
        return get_class(c)->scale(x, y, z);
      }
      static MutableClassPtr translate(sunray::script::MutableClassPtr& c, double x, double y, double z)
      {
        return get_class(c)->translate(x, y, z);
      }
      static MutableClassPtr rotate_x(sunray::script::MutableClassPtr& c, double radians)
      {
        return get_class(c)->rotate_x(radians);
      }
      static MutableClassPtr rotate_y(sunray::script::MutableClassPtr& c, double radians)
      {
        return get_class(c)->rotate_y(radians);
      }
      static MutableClassPtr rotate_z(sunray::script::MutableClassPtr& c, double radians)
      {
        return get_class(c)->rotate_z(radians);
      }
      static MutableClassPtr shear(sunray::script::MutableClassPtr& c, double xy, double xz, double yx, double yz, double zx,
                                   double zy)
      {
        return get_class(c)->shear(xy, xz, yx, yz, zx, zy);
      }
      static double casts_shadow(sunray::script::MutableClassPtr& c, bool value)
      {
        get_class(c)->casts_shadow(value);
        return 0.0;
      }
    };
  }
}
