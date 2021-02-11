//
//  light.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 15.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/light.h>
#include <sun_ray/script/class.h>
#include <sun_ray/script/meta_class.h>
#include <sun_ray/script/objects/color.h>
#include <sun_ray/script/objects/point.h>


namespace sunray
{
  namespace script
  {
    class Light : public Class
    {
    public:
      Light(MetaClassPtr meta_class, const Point& point, const Color& intensity)
      : Class(meta_class)
      , light_{new PointLight{point.point(), intensity.color()}}
      {
      }

      Light(MetaClassPtr meta_class)
      : Class(meta_class)
      , light_{new PointLight{sunray::create_point(-10.0, 10.0, -10.0), sunray::Color{1, 1, 1}}}
      {
      }

      void position(const Point& position)
      {
        light_.reset(new PointLight{position.point(), light_->intensity()});
      }

      void intensity(const Color& intensity)
      {
        light_.reset(new PointLight{light_->position(), intensity.color()});
      }

      std::string to_string() const override
      {
        std::stringstream ss_pos;
        ss_pos << light_->position();
        std::stringstream ss_int;
        ss_int << light_->intensity();

        return fmt::format("Light p: {{{}}} i: {{{}}}", ss_pos.str(), ss_int.str());
      }

      std::shared_ptr<const sunray::PointLight> light() const
      {
        return light_;
      }

    private:
      std::shared_ptr<const sunray::PointLight> light_;
    };


    class LightMetaClass : public sunray::script::MetaClass<Light>
    {
    public:
      LightMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Light";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<LightMetaClass>(shared_from_this());
        registry.add_variadic_function("Light_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.empty()) {
            return self->construct();
          } else if (parameter.size() != 2) {
            throw std::runtime_error{
              fmt::format("Light constructor called with wrong parameter count. Should be 2, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]), as_class(parameter[1]));
        });
        registry.add_function("Light_set_position", position);
        registry.add_function("Light_set_intensity", intensity);
      }

      std::shared_ptr<Light> construct(const sunray::script::MutableClassPtr& position,
                                       const sunray::script::MutableClassPtr& color) const
      {
        return std::make_shared<Light>(shared_from_this(), cast_object<Point, PointMetaClass>(position, "position"),
                                       cast_object<Color, PointMetaClass>(color, "color"));
      }

      std::shared_ptr<Light> construct() const
      {
        return std::make_shared<Light>(shared_from_this());
      }

    private:
      static double position(sunray::script::MutableClassPtr& c, const sunray::script::MutableClassPtr& point)
      {
        get_class(c)->position(cast_object<Point>(point, "Light", "position"));
        return 0.0;
      }
      static double intensity(sunray::script::MutableClassPtr& c, const sunray::script::MutableClassPtr& color)
      {
        get_class(c)->intensity(cast_object<Color>(color, "Light", "intensity"));
        return 0.0;
      }
    };
  }
}
