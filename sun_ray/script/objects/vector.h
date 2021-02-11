//
//  vector.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 14.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/tuple.h>
#include <sun_ray/script/class.h>
#include <sun_ray/script/meta_class.h>


namespace sunray
{
  namespace script
  {
    class Vector : public Class
    {
    public:
      Vector(MetaClassPtr meta_class, double x, double y, double z)
      : Class(meta_class)
      , vector_{sunray::create_vector(x, y, z)}
      {
      }

      Vector(MetaClassPtr meta_class, sunray::Vector&& v)
      : Class(meta_class)
      , vector_{v}
      {
      }

      double x() const
      {
        return vector_.x();
      }

      double y() const
      {
        return vector_.y();
      }

      double z() const
      {
        return vector_.z();
      }

      MutableClassPtr add(const sunray::script::Vector& v) const
      {
        return std::make_shared<Vector>(meta_class(), vector_ + v.vector_);
      }

      MutableClassPtr normalize() const
      {
        return std::make_shared<Vector>(meta_class(), vector_.normalize());
      }

      MutableClassPtr multiply(double scalar) const
      {
        return std::make_shared<Vector>(meta_class(), vector_ * scalar);
      }

      std::string to_string() const override
      {
        return fmt::format("Vector x: {} y: {} z: {}", x(), y(), z());
      }

      const sunray::Vector& vector() const
      {
        return vector_;
      }

    private:
      friend class Point;
      sunray::Vector vector_;
    };


    class VectorMetaClass : public sunray::script::MetaClass<Vector>
    {
    public:
      VectorMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Vector";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<VectorMetaClass>(shared_from_this());
        registry.add_variadic_function("Vector_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 3) {
            throw std::runtime_error{
              fmt::format("Vector constructor called with wrong parameter count. Should be 3, but is {}.", parameter.size())};
          }
          return self->construct(as_double(parameter[0]), as_double(parameter[1]), as_double(parameter[2]));
        });
        registry.add_function("Vector_normalize", normalize);
        registry.add_function("Vector_multiply", multiply);
        registry.add_function("Vector_add", add);
      }

      std::shared_ptr<Vector> construct(double x, double y, double z) const
      {
        return std::make_shared<Vector>(shared_from_this(), x, y, z);
      }

    private:
      static MutableClassPtr normalize(const sunray::script::MutableClassPtr& c)
      {
        return get_class(c)->normalize();
      }

      static MutableClassPtr multiply(const sunray::script::MutableClassPtr& c, double scalar)
      {
        return get_class(c)->multiply(scalar);
      }

      static MutableClassPtr add(const sunray::script::MutableClassPtr& c, const sunray::script::MutableClassPtr& v)
      {
        auto vec = dynamic_cast<const Vector*>(v.get());
        if (!vec) {
          throw std::runtime_error{fmt::format("Vector add wrong parameter for vector. Should be Vector but is {}.",
                                               v ? v->meta_class()->name() : "null")};
        }

        return get_class(c)->add(*vec);
      }
    };
  }
}
