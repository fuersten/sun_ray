//
//  cube.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 20.04.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/cube.h>
#include <sun_ray/script/objects/object.h>


namespace sunray
{
  namespace script
  {
    class Cube : public Object
    {
    public:
      Cube(MetaClassPtr meta_class, const Material& material)
      : Object(meta_class, material)
      {
      }

      std::string to_string() const override
      {
        return fmt::format("Cube");
      }

      sunray::CubePtr cube() const
      {
        return sunray::Cube::make_cube(material_, trans_.matrix(), casts_shadow_);
      }

      std::shared_ptr<const sunray::Object> shape() const override
      {
        return cube();
      }
    };


    class CubeMetaClass : public ObjectMetaClass
    {
    public:
      CubeMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Cube";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<CubeMetaClass>(shared_from_this());
        registry.add_variadic_function("Cube_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 1) {
            throw std::runtime_error{
              fmt::format("Cube constructor called with wrong parameter count. Should be 1, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]));
        });
        registry.add_function("Cube_scale", scale);
        registry.add_function("Cube_shear", shear);
        registry.add_function("Cube_translate", translate);
        registry.add_function("Cube_rotate_x", rotate_x);
        registry.add_function("Cube_rotate_y", rotate_y);
        registry.add_function("Cube_rotate_z", rotate_z);
        registry.add_function("Cube_set_casts_shadow", casts_shadow);
      }

      std::shared_ptr<Cube> construct(const sunray::script::MutableClassPtr& material) const
      {
        return std::make_shared<Cube>(shared_from_this(), cast_object<Material, MaterialMetaClass>(material, "material"));
      }
    };
  }
}
