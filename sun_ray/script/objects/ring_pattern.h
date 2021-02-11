//
//  ring_pattern.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 07.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/ring_pattern.h>
#include <sun_ray/script/objects/color.h>
#include <sun_ray/script/objects/pattern.h>


namespace sunray
{
  namespace script
  {
    class RingPattern : public Pattern
    {
    public:
      RingPattern(MetaClassPtr meta_class, const Color& a, const Color& b)
      : Pattern(meta_class)
      , a_{a.color()}
      , b_{b.color()}
      {
      }

      std::string to_string() const override
      {
        return fmt::format("RingPattern");
      }

      sunray::PatternPtr pattern() const override
      {
        return std::make_shared<sunray::RingPattern>(a_, b_, trans_.matrix());
      }

    private:
      sunray::Color a_;
      sunray::Color b_;
    };


    class RingPatternMetaClass : public PatternMetaClass
    {
    public:
      RingPatternMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "RingPattern";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<RingPatternMetaClass>(shared_from_this());
        registry.add_variadic_function("RingPattern_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 2) {
            throw std::runtime_error{
              fmt::format("Plane constructor called with wrong parameter count. Should be 2, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]), as_class(parameter[1]));
        });
        registry.add_function("RingPattern_scale", scale);
        registry.add_function("RingPattern_shear", shear);
        registry.add_function("RingPattern_translate", translate);
        registry.add_function("RingPattern_rotate_x", rotate_x);
        registry.add_function("RingPattern_rotate_y", rotate_y);
        registry.add_function("RingPattern_rotate_z", rotate_z);
      }

      std::shared_ptr<RingPattern> construct(const sunray::script::MutableClassPtr& color_a,
                                             const sunray::script::MutableClassPtr& color_b) const
      {
        return std::make_shared<RingPattern>(shared_from_this(), cast_object<Color, ColorMetaClass>(color_a, "color"),
                                             cast_object<Color, ColorMetaClass>(color_b, "color"));
      }
    };
  }
}
