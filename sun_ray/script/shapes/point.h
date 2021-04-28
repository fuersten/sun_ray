//
//  point.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 15.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/tuple.h>
#include <sun_ray/script/class.h>
#include <sun_ray/script/meta_class.h>
#include <sun_ray/script/shapes/vector.h>


namespace sunray
{
  namespace script
  {
    class Point : public Class
    {
    public:
      Point(MetaClassPtr meta_class, double x, double y, double z)
      : Class(meta_class)
      , point_{sunray::create_point(x, y, z)}
      {
      }

      Point(MetaClassPtr meta_class, sunray::Point&& p)
      : Class(meta_class)
      , point_{p}
      {
      }

      double x() const
      {
        return point_.x();
      }

      double y() const
      {
        return point_.y();
      }

      double z() const
      {
        return point_.z();
      }

      MutableClassPtr add(const sunray::script::Vector& v) const
      {
        return std::make_shared<Point>(meta_class(), point_ + v.vector_);
      }

      std::string to_string() const override
      {
        return fmt::format("Point x: {} y: {} z: {}", x(), y(), z());
      }

      const sunray::Point& point() const
      {
        return point_;
      }

    private:
      sunray::Point point_;
    };


    class PointMetaClass : public sunray::script::MetaClass<Point>
    {
    public:
      PointMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Point";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<PointMetaClass>(shared_from_this());
        registry.add_variadic_function("Point_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 3) {
            throw std::runtime_error{
              fmt::format("Point constructor called with wrong parameter count. Should be 3, but is {}.", parameter.size())};
          }
          return self->construct(as_double(parameter[0]), as_double(parameter[1]), as_double(parameter[2]));
        });
        registry.add_function("Point_add", add);
        registry.add_function("Point_get_x", get_x);
        registry.add_function("Point_get_y", get_y);
        registry.add_function("Point_get_z", get_z);
      }

      std::shared_ptr<Point> construct(double x, double y, double z) const
      {
        return std::make_shared<Point>(shared_from_this(), x, y, z);
      }

    private:
      static double get_x(const sunray::script::MutableClassPtr& c)
      {
        return get_class(c)->x();
      }

      static double get_y(const sunray::script::MutableClassPtr& c)
      {
        return get_class(c)->y();
      }

      static double get_z(const sunray::script::MutableClassPtr& c)
      {
        return get_class(c)->z();
      }

      static MutableClassPtr add(const sunray::script::MutableClassPtr& c, const sunray::script::MutableClassPtr& v)
      {
        auto vec = dynamic_cast<const Vector*>(v.get());
        if (!vec) {
          throw std::runtime_error{fmt::format("Point add wrong parameter for vector. Should be Vector but is {}.",
                                               v ? v->meta_class()->name() : "null")};
        }

        return get_class(c)->add(*vec);
      }
    };
  }
}
