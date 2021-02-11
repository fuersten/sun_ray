//
//  disk.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 19.04.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/disk.h>
#include <sun_ray/script/objects/object.h>


namespace sunray
{
  namespace script
  {
    class Disk : public Object
    {
    public:
      Disk(MetaClassPtr meta_class, const Material& material)
      : Object(meta_class, material)
      {
      }

      std::string to_string() const override
      {
        return fmt::format("Disk");
      }

      sunray::DiskPtr disk(double inner_radius) const
      {
        return sunray::Disk::make_disk(material_, trans_.matrix(), casts_shadow_, inner_radius);
      }

      std::shared_ptr<const sunray::Object> shape() const override
      {
        return disk(inner_radius_);
      }

      void inner_radius(double inner_radius)
      {
        inner_radius_ = inner_radius;
      }

    private:
      double inner_radius_{0.0};
    };


    class DiskMetaClass : public ObjectMetaClass
    {
    public:
      DiskMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Disk";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<DiskMetaClass>(shared_from_this());
        registry.add_variadic_function("Disk_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 1) {
            throw std::runtime_error{
              fmt::format("Disk constructor called with wrong parameter count. Should be 1, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]));
        });
        registry.add_function("Disk_scale", scale);
        registry.add_function("Disk_shear", shear);
        registry.add_function("Disk_translate", translate);
        registry.add_function("Disk_rotate_x", rotate_x);
        registry.add_function("Disk_rotate_y", rotate_y);
        registry.add_function("Disk_rotate_z", rotate_z);
        registry.add_function("Disk_set_casts_shadow", casts_shadow);
        registry.add_function("Disk_set_inner_radius", inner_radius);
      }

      std::shared_ptr<Disk> construct(const sunray::script::MutableClassPtr& material) const
      {
        return std::make_shared<Disk>(shared_from_this(), cast_object<Material, MaterialMetaClass>(material, "material"));
      }

    private:
      static double inner_radius(sunray::script::MutableClassPtr& c, double inner_radius)
      {
        std::dynamic_pointer_cast<Disk>(c)->inner_radius(inner_radius);
        return 0.0;
      }
    };
  }
}
