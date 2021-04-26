//
//  cylinder.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 25.04.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/cylinder.h>
#include <sun_ray/script/objects/shape.h>


namespace sunray
{
  namespace script
  {
    class Cylinder : public Shape
    {
    public:
      Cylinder(MetaClassPtr meta_class, const Material& material)
      : Shape(meta_class, material)
      {
      }

      std::string to_string() const override
      {
        return fmt::format("Cylinder max: {} min: {} closed: {}", maximum_, minimum_, closed_);
      }

      sunray::CylinderPtr cylinder(double maximum, double minimum, bool closed) const
      {
        return sunray::Cylinder::make_cylinder(material_, trans_.matrix(), casts_shadow_, maximum, minimum, closed);
      }

      std::shared_ptr<const sunray::Object> shape() const override
      {
        return cylinder(maximum_, minimum_, closed_);
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


    class CylinderMetaClass : public ShapeMetaClass
    {
    public:
      CylinderMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Cylinder";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<CylinderMetaClass>(shared_from_this());
        registry.add_variadic_function("Cylinder_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 1) {
            throw std::runtime_error{
              fmt::format("Cylinder constructor called with wrong parameter count. Should be 1, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]));
        });
        registry.add_function("Cylinder_scale", scale);
        registry.add_function("Cylinder_shear", shear);
        registry.add_function("Cylinder_translate", translate);
        registry.add_function("Cylinder_rotate_x", rotate_x);
        registry.add_function("Cylinder_rotate_y", rotate_y);
        registry.add_function("Cylinder_rotate_z", rotate_z);
        registry.add_function("Cylinder_set_casts_shadow", casts_shadow);
        registry.add_function("Cylinder_set_maximum", maximum);
        registry.add_function("Cylinder_set_minimum", minimum);
        registry.add_function("Cylinder_set_closed", closed);
      }

      std::shared_ptr<Cylinder> construct(const sunray::script::MutableClassPtr& material) const
      {
        return std::make_shared<Cylinder>(shared_from_this(), cast_object<Material, MaterialMetaClass>(material, "material"));
      }

    private:
      static double maximum(sunray::script::MutableClassPtr& c, double maximum)
      {
        std::dynamic_pointer_cast<Cylinder>(c)->maximum(maximum);
        return 0.0;
      }
      static double minimum(sunray::script::MutableClassPtr& c, double minimum)
      {
        std::dynamic_pointer_cast<Cylinder>(c)->minimum(minimum);
        return 0.0;
      }
      static double closed(sunray::script::MutableClassPtr& c, bool closed)
      {
        std::dynamic_pointer_cast<Cylinder>(c)->closed(closed);
        return 0.0;
      }
    };
  }
}
