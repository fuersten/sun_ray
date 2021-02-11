//
//  color.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 08.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/color.h>
#include <sun_ray/script/class.h>
#include <sun_ray/script/meta_class.h>


namespace sunray
{
  namespace script
  {
    class Color : public Class
    {
    public:
      Color(MetaClassPtr meta_class)
      : Class(meta_class)
      {
      }

      Color(MetaClassPtr meta_class, double red, double green, double blue)
      : Class(meta_class)
      , red_{red}
      , green_{green}
      , blue_{blue}
      {
      }

      double red() const
      {
        return red_;
      }

      double green() const
      {
        return green_;
      }

      double blue() const
      {
        return blue_;
      }

      void red(double red)
      {
        red_ = red;
      }

      void green(double green)
      {
        green_ = green;
      }

      void blue(double blue)
      {
        blue_ = blue;
      }

      std::string to_string() const override
      {
        return fmt::format("Color r: {} g: {} b: {}", red_, green_, blue_);
      }

      sunray::Color color() const
      {
        return sunray::Color{static_cast<float>(red_), static_cast<float>(green_), static_cast<float>(blue_)};
      }

    private:
      double red_{0.0};
      double green_{0.0};
      double blue_{0.0};
    };


    class ColorMetaClass : public sunray::script::MetaClass<Color>
    {
    public:
      ColorMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Color";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<ColorMetaClass>(shared_from_this());
        registry.add_variadic_function("Color_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.empty()) {
            return self->construct();
          } else if (parameter.size() != 3) {
            throw std::runtime_error{
              fmt::format("Color constructor called with wrong parameter count. Should be 0 or 3, but is {}.", parameter.size())};
          }
          auto red = as_double(parameter[0]);
          auto green = as_double(parameter[1]);
          auto blue = as_double(parameter[2]);
          return self->construct(red, green, blue);
        });
        registry.add_function("Color_get_red", get_red);
        registry.add_function("Color_get_green", get_green);
        registry.add_function("Color_get_blue", get_blue);

        registry.add_function("Color_set_red", set_red);
        registry.add_function("Color_set_green", set_green);
        registry.add_function("Color_set_blue", set_blue);
      }

      std::shared_ptr<Color> construct() const
      {
        return std::make_shared<Color>(shared_from_this());
      }

      std::shared_ptr<Color> construct(double red, double green, double blue) const
      {
        return std::make_shared<Color>(shared_from_this(), red, green, blue);
      }

    private:
      static double get_red(const sunray::script::MutableClassPtr& c)
      {
        return get_class(c)->red();
      }
      static double get_green(const sunray::script::MutableClassPtr& c)
      {
        return get_class(c)->green();
      }
      static double get_blue(const sunray::script::MutableClassPtr& c)
      {
        return get_class(c)->blue();
      }

      static double set_red(sunray::script::MutableClassPtr& c, double red)
      {
        get_class(c)->red(red);
        return 0;
      }
      static double set_green(sunray::script::MutableClassPtr& c, double green)
      {
        get_class(c)->green(green);
        return 0;
      }
      static double set_blue(sunray::script::MutableClassPtr& c, double blue)
      {
        get_class(c)->blue(blue);
        return 0;
      }
    };
  }
}
