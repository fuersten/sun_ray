//
//  plane.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 04.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/plane.h>
#include <sun_ray/script/shapes/shape.h>


namespace sunray
{
  namespace script
  {
    class Plane : public Shape
    {
    public:
      Plane(MetaClassPtr meta_class, const Material& material)
      : Shape(meta_class, material)
      {
      }

      std::string to_string() const override
      {
        return fmt::format("Plane");
      }

      sunray::PlanePtr plane() const
      {
        return sunray::Plane::make_plane(material_, trans_.matrix(), casts_shadow_);
      }

      std::shared_ptr<const sunray::Shape> shape() const override
      {
        return plane();
      }
    };


    class PlaneMetaClass : public ShapeMetaClass
    {
    public:
      PlaneMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Plane";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<PlaneMetaClass>(shared_from_this());
        registry.add_variadic_function("Plane_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 1) {
            throw std::runtime_error{
              fmt::format("Plane constructor called with wrong parameter count. Should be 1, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]));
        });
        registry.add_function("Plane_scale", scale);
        registry.add_function("Plane_shear", shear);
        registry.add_function("Plane_translate", translate);
        registry.add_function("Plane_rotate_x", rotate_x);
        registry.add_function("Plane_rotate_y", rotate_y);
        registry.add_function("Plane_rotate_z", rotate_z);
        registry.add_function("Plane_set_casts_shadow", casts_shadow);
      }

      std::shared_ptr<Plane> construct(const sunray::script::MutableClassPtr& material) const
      {
        return std::make_shared<Plane>(shared_from_this(), cast_object<Material, MaterialMetaClass>(material, "material"));
      }
    };
  }
}
