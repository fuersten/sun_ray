//
//  gradient_pattern.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 06.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/gradient_pattern.h>
#include <sun_ray/script/objects/color.h>
#include <sun_ray/script/objects/pattern.h>


namespace sunray
{
  namespace script
  {
    class GradientPattern : public Pattern
    {
    public:
      GradientPattern(MetaClassPtr meta_class, const Color& a, const Color& b)
      : Pattern(meta_class)
      , a_{a.color()}
      , b_{b.color()}
      {
      }

      std::string to_string() const override
      {
        return fmt::format("GradientPattern");
      }

      sunray::PatternPtr pattern() const override
      {
        return std::make_shared<sunray::GradientPattern>(a_, b_, trans_.matrix());
      }

    private:
      sunray::Color a_;
      sunray::Color b_;
    };


    class GradientPatternMetaClass : public PatternMetaClass
    {
    public:
      GradientPatternMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "GradientPattern";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<GradientPatternMetaClass>(shared_from_this());
        registry.add_variadic_function("GradientPattern_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 2) {
            throw std::runtime_error{
              fmt::format("Plane constructor called with wrong parameter count. Should be 2, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]), as_class(parameter[1]));
        });
        registry.add_function("GradientPattern_scale", scale);
        registry.add_function("GradientPattern_shear", shear);
        registry.add_function("GradientPattern_translate", translate);
        registry.add_function("GradientPattern_rotate_x", rotate_x);
        registry.add_function("GradientPattern_rotate_y", rotate_y);
        registry.add_function("GradientPattern_rotate_z", rotate_z);
      }

      std::shared_ptr<GradientPattern> construct(const sunray::script::MutableClassPtr& color_a,
                                                 const sunray::script::MutableClassPtr& color_b) const
      {
        return std::make_shared<GradientPattern>(shared_from_this(), cast_object<Color, ColorMetaClass>(color_a, "color"),
                                                 cast_object<Color, ColorMetaClass>(color_b, "color"));
      }
    };
  }
}
