//
//  world.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 18.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/world.h>
#include <sun_ray/script/class.h>
#include <sun_ray/script/meta_class.h>
#include <sun_ray/script/shapes/light.h>
#include <sun_ray/script/shapes/sphere.h>


namespace sunray
{
  namespace script
  {
    class World
    : public Class
    , public std::enable_shared_from_this<World>
    {
    public:
      World(MetaClassPtr meta_class)
      : Class(meta_class)
      {
      }

      MutableClassPtr add(const std::shared_ptr<const sunray::PointLight>& light)
      {
        world_.add_light(light);
        return shared_from_this();
      }

      MutableClassPtr add(const std::shared_ptr<const sunray::Shape>& object)
      {
        world_.add_object(object);
        return shared_from_this();
      }

      void shadows(bool set)
      {
        shadows_ = set;
      }

      void reflections(bool set)
      {
        reflections_ = set;
      }

      void refractions(bool set)
      {
        refractions_ = set;
      }

      std::string to_string() const override
      {
        return fmt::format("World");
      }

      const sunray::World& world() const
      {
        sunray::RenderContext context;
        context.shadows_ = shadows_;
        context.reflections_ = reflections_;
        context.refractions_ = refractions_;
        world_.context(context);
        return world_;
      }

    private:
      bool shadows_{true};
      bool reflections_{true};
      bool refractions_{true};
      mutable sunray::World world_;
    };


    class WorldMetaClass : public sunray::script::MetaClass<World>
    {
    public:
      WorldMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "World";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<WorldMetaClass>(shared_from_this());
        registry.add_variadic_function("World_constructor", [self](const std::vector<Variant>& parameter) {
          if (!parameter.empty()) {
            throw std::runtime_error{
              fmt::format("World constructor called with wrong parameter count. Should be 0, but is {}.", parameter.size())};
          }
          return self->construct();
        });
        registry.add_function("World_add", add);
        registry.add_function("World_set_shadows", shadows);
        registry.add_function("World_set_reflections", reflections);
        registry.add_function("World_set_refractions", refractions);
      }

      std::shared_ptr<World> construct() const
      {
        return std::make_shared<World>(shared_from_this());
      }

    private:
      static MutableClassPtr add(MutableClassPtr& c, const MutableClassPtr& obj)
      {
        auto light = std::dynamic_pointer_cast<Light>(obj);
        if (light) {
          return get_class(c)->add(light->light());
        }
        auto shape = std::dynamic_pointer_cast<Shape>(obj);
        if (shape) {
          return get_class(c)->add(shape->shape());
        }
        throw std::runtime_error{"World add has to be called with light or a shape"};
      }
      static double shadows(MutableClassPtr& c, bool set)
      {
        get_class(c)->shadows(set);
        return 0.0;
      }
      static double reflections(MutableClassPtr& c, bool set)
      {
        get_class(c)->reflections(set);
        return 0.0;
      }
      static double refractions(MutableClassPtr& c, bool set)
      {
        get_class(c)->refractions(set);
        return 0.0;
      }
    };
  }
}
