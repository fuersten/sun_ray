//
//  triangle.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 09.06.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/triangle.h>
#include <sun_ray/script/objects/point.h>
#include <sun_ray/script/objects/shape.h>


namespace sunray
{
  namespace script
  {
    class Triangle : public Shape
    {
    public:
      Triangle(MetaClassPtr meta_class, const Material& material, const Point& point1, const Point& point2, const Point& point3)
      : Shape(meta_class, material)
      , point1_{point1}
      , point2_{point2}
      , point3_{point3}
      {
      }

      std::string to_string() const override
      {
        return fmt::format("Triangle");
      }

      sunray::TrianglePtr triangle() const
      {
        return sunray::Triangle::make_triangle(material_, trans_.matrix(), point1_.point(), point2_.point(), point3_.point(),
                                               casts_shadow_);
      }

      std::shared_ptr<const sunray::Object> shape() const override
      {
        return triangle();
      }

    private:
      const Point point1_;
      const Point point2_;
      const Point point3_;
    };


    class TriangleMetaClass : public ShapeMetaClass
    {
    public:
      TriangleMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Triangle";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<TriangleMetaClass>(shared_from_this());
        registry.add_variadic_function("Triangle_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 4) {
            throw std::runtime_error{
              fmt::format("Triangle constructor called with wrong parameter count. Should be 4, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]), as_class(parameter[1]), as_class(parameter[2]), as_class(parameter[3]));
        });
        registry.add_function("Triangle_scale", scale);
        registry.add_function("Triangle_shear", shear);
        registry.add_function("Triangle_translate", translate);
        registry.add_function("Triangle_rotate_x", rotate_x);
        registry.add_function("Triangle_rotate_y", rotate_y);
        registry.add_function("Triangle_rotate_z", rotate_z);
        registry.add_function("Triangle_set_casts_shadow", casts_shadow);
      }

      std::shared_ptr<Triangle> construct(const sunray::script::MutableClassPtr& material,
                                          const sunray::script::MutableClassPtr& point1,
                                          const sunray::script::MutableClassPtr& point2,
                                          const sunray::script::MutableClassPtr& point3) const
      {
        return std::make_shared<Triangle>(shared_from_this(), cast_object<Material, MaterialMetaClass>(material, "material"),
                                          cast_object<Point, PointMetaClass>(point1, "point1"),
                                          cast_object<Point, PointMetaClass>(point2, "point2"),
                                          cast_object<Point, PointMetaClass>(point3, "point3"));
      }
    };
  }
}
