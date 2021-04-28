//
//  cone.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 27.04.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/cone.h>
#include <sun_ray/script/shapes/shape.h>


namespace sunray
{
  namespace script
  {
    class Cone : public Shape
    {
    public:
      Cone(MetaClassPtr meta_class, const Material& material)
      : Shape(meta_class, material)
      {
      }

      std::string to_string() const override
      {
        return fmt::format("Cone max: {} min: {} closed: {}", maximum_, minimum_, closed_);
      }

      sunray::ConePtr cone(double maximum, double minimum, bool closed) const
      {
        return sunray::Cone::make_cone(material_, trans_.matrix(), casts_shadow_, maximum, minimum, closed);
      }

      std::shared_ptr<const sunray::Shape> shape() const override
      {
        return cone(maximum_, minimum_, closed_);
      }

      void maximum(double maximum)
      {
        maximum_ = maximum;
      }

      double maximum() const
      {
        return maximum_;
      }

      void minimum(double minimum)
      {
        minimum_ = minimum;
      }

      double minimum() const
      {
        return minimum_;
      }

      void closed(bool closed)
      {
        closed_ = closed;
      }

      bool closed() const
      {
        return closed_;
      }

    private:
      double maximum_{std::numeric_limits<double>::infinity()};
      double minimum_{-std::numeric_limits<double>::infinity()};
      bool closed_{false};
    };


    class ConeMetaClass : public ShapeMetaClass
    {
    public:
      ConeMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Cone";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<ConeMetaClass>(shared_from_this());
        registry.add_variadic_function("Cone_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 1) {
            throw std::runtime_error{
              fmt::format("Cone constructor called with wrong parameter count. Should be 1, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]));
        });
        registry.add_function("Cone_scale", scale);
        registry.add_function("Cone_shear", shear);
        registry.add_function("Cone_translate", translate);
        registry.add_function("Cone_rotate_x", rotate_x);
        registry.add_function("Cone_rotate_y", rotate_y);
        registry.add_function("Cone_rotate_z", rotate_z);
        registry.add_function("Cone_set_casts_shadow", casts_shadow);
        registry.add_function("Cone_set_maximum", maximum);
        registry.add_function("Cone_set_minimum", minimum);
        registry.add_function("Cone_set_closed", closed);
      }

      std::shared_ptr<Cone> construct(const sunray::script::MutableClassPtr& material) const
      {
        return std::make_shared<Cone>(shared_from_this(), cast_object<Material, MaterialMetaClass>(material, "material"));
      }

    private:
      static double maximum(sunray::script::MutableClassPtr& c, double maximum)
      {
        std::dynamic_pointer_cast<Cone>(c)->maximum(maximum);
        return 0.0;
      }
      static double minimum(sunray::script::MutableClassPtr& c, double minimum)
      {
        std::dynamic_pointer_cast<Cone>(c)->minimum(minimum);
        return 0.0;
      }
      static double closed(sunray::script::MutableClassPtr& c, bool closed)
      {
        std::dynamic_pointer_cast<Cone>(c)->closed(closed);
        return 0.0;
      }
    };
  }
}
