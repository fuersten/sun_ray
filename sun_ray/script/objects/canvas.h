//
//  canvas.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 11.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/canvas_file_writer.h>
#include <sun_ray/feature/canvas.h>
#include <sun_ray/script/class.h>
#include <sun_ray/script/meta_class.h>
#include <sun_ray/script/objects/color.h>


namespace sunray
{
  namespace script
  {
    class Canvas : public Class
    {
    public:
      Canvas(MetaClassPtr meta_class, double width, double height)
      : Class(meta_class)
      , canvas_{static_cast<uint32_t>(width), static_cast<uint32_t>(height)}
      {
      }

      Canvas(MetaClassPtr meta_class, sunray::Canvas&& canvas)
      : Class(meta_class)
      , canvas_{std::move(canvas)}
      {
      }

      double width() const
      {
        return canvas_.width();
      }

      double height() const
      {
        return canvas_.height();
      }

      void set_pixel(double x, double y, const Color& color)
      {
        canvas_.pixel_at(
          static_cast<uint32_t>(x), static_cast<uint32_t>(y),
          sunray::Color{static_cast<float>(color.red()), static_cast<float>(color.green()), static_cast<float>(color.blue())});
      }

      void write(const std::string& filename)
      {
        sunray::CanvasFileWriter writer{sunray::ImageFormat::PNG, filename};
        writer.write(canvas_);
      }

      std::string to_string() const override
      {
        return fmt::format("Canvas w: {} h: {}", width(), height());
      }

    private:
      sunray::Canvas canvas_;
    };


    class CanvasMetaClass : public sunray::script::MetaClass<Canvas>
    {
    public:
      CanvasMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Canvas";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<CanvasMetaClass>(shared_from_this());
        registry.add_variadic_function("Canvas_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.size() != 2) {
            throw std::runtime_error{
              fmt::format("Canvas constructor called with wrong parameter count. Should be 2, but is {}.", parameter.size())};
          }
          return self->construct(as_double(parameter[0]), as_double(parameter[1]));
        });
        registry.add_function("Canvas_get_width", get_width);
        registry.add_function("Canvas_get_height", get_height);
        registry.add_function("Canvas_set_pixel", set_pixel);
        registry.add_function("Canvas_write", write);
      }

      std::shared_ptr<Canvas> construct(double width, double height) const
      {
        return std::make_shared<Canvas>(shared_from_this(), width, height);
      }

    private:
      static double get_width(const sunray::script::MutableClassPtr& c)
      {
        return get_class(c)->width();
      }

      static double get_height(const sunray::script::MutableClassPtr& c)
      {
        return get_class(c)->height();
      }

      static double set_pixel(sunray::script::MutableClassPtr& c, double x, double y,
                              const sunray::script::MutableClassPtr& color)
      {
        auto color_to_set = dynamic_cast<const Color*>(color.get());
        if (!color_to_set) {
          throw std::runtime_error{fmt::format("Canvas set_pixel wrong parameter for color. Should be Color but is {}.",
                                               color ? color->meta_class()->name() : "null")};
        }
        get_class(c)->set_pixel(x, y, *color_to_set);
        return 0;
      }
      static double write(sunray::script::MutableClassPtr& c, const std::string& filename)
      {
        get_class(c)->write(filename);
        return 0;
      }
    };
  }
}
