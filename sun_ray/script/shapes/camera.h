//
//  camera.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 18.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/camera.h>
#include <sun_ray/script/class.h>
#include <sun_ray/script/meta_class.h>
#include <sun_ray/script/shapes/canvas.h>
#include <sun_ray/script/shapes/point.h>
#include <sun_ray/script/shapes/vector.h>
#include <sun_ray/script/shapes/world.h>


namespace sunray
{
  namespace script
  {
    class Camera
    : public Class
    , public std::enable_shared_from_this<Camera>
    {
    public:
      Camera(MetaClassPtr meta_class)
      : Class(meta_class)
      {
      }

      MutableClassPtr from(const sunray::Point& from)
      {
        from_ = from;
        return shared_from_this();
      }

      MutableClassPtr to(const sunray::Point& to)
      {
        to_ = to;
        return shared_from_this();
      }

      MutableClassPtr up(const sunray::Vector& up)
      {
        up_ = up;
        return shared_from_this();
      }

      MutableClassPtr field_of_view(double fov)
      {
        field_of_view_ = fov;
        return shared_from_this();
      }

      MutableClassPtr horizontal(double h)
      {
        horizontal_ = static_cast<uint32_t>(h);
        return shared_from_this();
      }

      double horizontal() const
      {
        return horizontal_;
      }

      MutableClassPtr vertical(double v)
      {
        vertical_ = static_cast<uint32_t>(v);
        return shared_from_this();
      }

      std::string to_string() const override
      {
        std::stringstream ss_from;
        ss_from << from_;
        std::stringstream ss_to;
        ss_to << to_;
        std::stringstream ss_up;
        ss_up << up_;
        return fmt::format("Camera h: {} v: {} fov: {} f: {{{}}} t: {{{}}} u: {{{}}}", horizontal_, vertical_, field_of_view_,
                           ss_from.str(), ss_to.str(), ss_up.str());
      }

      MutableClassPtr render(const sunray::World& world) const
      {
        sunray::Camera camera{horizontal_, vertical_, field_of_view_, sunray::view_transformation(from_, to_, up_)};
        sunray::Canvas canvas = camera.render(world);
        return std::make_shared<Canvas>(std::make_shared<CanvasMetaClass>(), std::move(canvas));
      }

    private:
      uint32_t horizontal_{500};
      uint32_t vertical_{250};
      double field_of_view_{sunray::PI / 3};
      sunray::Point from_{sunray::create_point(0, 1.5, 0.7)};
      sunray::Point to_{sunray::create_point(0, 1, 0)};
      sunray::Vector up_{sunray::create_vector(0, 1, 0)};
    };


    class CameraMetaClass : public sunray::script::MetaClass<Camera>
    {
    public:
      CameraMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Camera";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<CameraMetaClass>(shared_from_this());
        registry.add_variadic_function("Camera_constructor", [self](const std::vector<Variant>& parameter) {
          if (!parameter.empty()) {
            throw std::runtime_error{
              fmt::format("Camera constructor called with wrong parameter count. Should be 0, but is {}.", parameter.size())};
          }
          return self->construct();
        });
        registry.add_function("Camera_render", render);
        registry.add_function("Camera_set_from", from);
        registry.add_function("Camera_set_to", to);
        registry.add_function("Camera_set_up", up);
        registry.add_function("Camera_set_field_of_view", fov);
        registry.add_function("Camera_set_horizontal", set_horizontal);
        registry.add_function("Camera_get_horizontal", get_horizontal);
        registry.add_function("Camera_set_vertical", set_vertical);
      }

      std::shared_ptr<Camera> construct() const
      {
        return std::make_shared<Camera>(shared_from_this());
      }

    private:
      static MutableClassPtr render(MutableClassPtr& c, const MutableClassPtr& world)
      {
        const auto& w = cast_object<World, WorldMetaClass>(world, "world");
        return get_class(c)->render(w.world());
      }
      static MutableClassPtr from(MutableClassPtr& c, const MutableClassPtr& point)
      {
        const auto& p = cast_object<Point, PointMetaClass>(point, "from");
        return get_class(c)->from(p.point());
      }
      static MutableClassPtr to(MutableClassPtr& c, const MutableClassPtr& point)
      {
        const auto& p = cast_object<Point, PointMetaClass>(point, "to");
        return get_class(c)->to(p.point());
      }
      static MutableClassPtr up(MutableClassPtr& c, const MutableClassPtr& vector)
      {
        const auto& v = cast_object<Vector, VectorMetaClass>(vector, "up");
        return get_class(c)->up(v.vector());
      }
      static MutableClassPtr fov(MutableClassPtr& c, double fov)
      {
        return get_class(c)->field_of_view(fov);
      }
      static MutableClassPtr set_horizontal(MutableClassPtr& c, double h)
      {
        return get_class(c)->horizontal(h);
      }
      static double get_horizontal(MutableClassPtr& c)
      {
        return get_class(c)->horizontal();
      }
      static MutableClassPtr set_vertical(MutableClassPtr& c, double v)
      {
        return get_class(c)->vertical(v);
      }
    };
  }
}
