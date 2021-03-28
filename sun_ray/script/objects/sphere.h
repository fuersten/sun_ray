//
//  sphere.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 16.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/sphere.h>
#include <sun_ray/script/objects/shape.h>


namespace sunray
{
  namespace script
  {
    class Sphere : public Shape
    {
    public:
      Sphere(MetaClassPtr meta_class, const Material& material)
      : Shape(meta_class, material)
      {
      }

      std::string to_string() const override
      {
        return fmt::format("Sphere");
      }

      sunray::SpherePtr sphere() const
      {
        return sunray::Sphere::make_sphere(material_, trans_.matrix(), casts_shadow_);
      }

      std::shared_ptr<const sunray::Object> shape() const override
      {
        return sphere();
      }
    };


    class SphereMetaClass : public ShapeMetaClass
    {
    public:
      SphereMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Sphere";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<SphereMetaClass>(shared_from_this());
        registry.add_variadic_function("Sphere_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 1) {
            throw std::runtime_error{
              fmt::format("Sphere constructor called with wrong parameter count. Should be 1, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]));
        });
        registry.add_function("Sphere_scale", scale);
        registry.add_function("Sphere_shear", shear);
        registry.add_function("Sphere_translate", translate);
        registry.add_function("Sphere_rotate_x", rotate_x);
        registry.add_function("Sphere_rotate_y", rotate_y);
        registry.add_function("Sphere_rotate_z", rotate_z);
        registry.add_function("Sphere_set_casts_shadow", casts_shadow);
      }

      std::shared_ptr<Sphere> construct(const sunray::script::MutableClassPtr& material) const
      {
        return std::make_shared<Sphere>(shared_from_this(), cast_object<Material, MaterialMetaClass>(material, "material"));
      }
    };
  }
}
