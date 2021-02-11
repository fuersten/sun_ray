//
//  checker_pattern.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 07.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/checker_pattern.h>
#include <sun_ray/script/objects/color.h>
#include <sun_ray/script/objects/pattern.h>


namespace sunray
{
  namespace script
  {
    class CheckerPattern : public Pattern
    {
    public:
      CheckerPattern(MetaClassPtr meta_class, const Color& a, const Color& b)
      : Pattern(meta_class)
      , a_{a.color()}
      , b_{b.color()}
      {
      }

      std::string to_string() const override
      {
        return fmt::format("CheckerPattern");
      }

      sunray::PatternPtr pattern() const override
      {
        return std::make_shared<sunray::CheckerPattern>(a_, b_, trans_.matrix());
      }

    private:
      sunray::Color a_;
      sunray::Color b_;
    };


    class CheckerPatternMetaClass : public PatternMetaClass
    {
    public:
      CheckerPatternMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "CheckerPattern";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<CheckerPatternMetaClass>(shared_from_this());
        registry.add_variadic_function("CheckerPattern_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 2) {
            throw std::runtime_error{
              fmt::format("Plane constructor called with wrong parameter count. Should be 2, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]), as_class(parameter[1]));
        });
        registry.add_function("CheckerPattern_scale", scale);
        registry.add_function("CheckerPattern_shear", shear);
        registry.add_function("CheckerPattern_translate", translate);
        registry.add_function("CheckerPattern_rotate_x", rotate_x);
        registry.add_function("CheckerPattern_rotate_y", rotate_y);
        registry.add_function("CheckerPattern_rotate_z", rotate_z);
      }

      std::shared_ptr<CheckerPattern> construct(const sunray::script::MutableClassPtr& color_a,
                                                const sunray::script::MutableClassPtr& color_b) const
      {
        return std::make_shared<CheckerPattern>(shared_from_this(), cast_object<Color, ColorMetaClass>(color_a, "color"),
                                                cast_object<Color, ColorMetaClass>(color_b, "color"));
      }
    };
  }
}
