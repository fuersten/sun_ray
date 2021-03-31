//
//  world.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 18.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/intersection_state.h>
#include <sun_ray/feature/light.h>

#include <numeric>
#include <thread>


namespace sunray
{
  class Object;
  using ObjectPtr = std::shared_ptr<const Object>;

  struct RenderContext {
    bool shadows_{true};
    bool reflections_{true};
    bool refractions_{true};
    uint8_t maximum_depth_{5};
    uint32_t number_of_threads_{std::thread::hardware_concurrency()};
  };


  class World
  {
  public:
    World() = default;

    ~World() = default;

    World(const World&) = delete;
    World(World&&) = default;
    World& operator=(const World&) = delete;
    World& operator=(World&&) = delete;

    void context(const RenderContext& context)
    {
      context_ = context;
    }

    const RenderContext& context() const
    {
      return context_;
    }

    void add_light(const LightPtr& light)
    {
      lights_.emplace_back(light);
    }

    void add_object(const ObjectPtr& object)
    {
      has_non_shadowed_object_ |= object->casts_shadow();
      objects_.emplace_back(object);
    }

    inline Color color_at(const Ray& ray, Intersections& intersections) const
    {
      return internal_color_at(ray, intersections, context_.maximum_depth_);
    }

    bool contains(const ObjectPtr object) const
    {
      auto it = std::find_if(objects_.begin(), objects_.end(), [&object](const auto& obj) {
        return obj == object;
      });
      return it != objects_.end();
    }

    void intersect(const Ray& ray, Intersections& intersections) const
    {
      std::for_each(objects_.begin(), objects_.end(), [&intersections, &ray](const auto& object) {
        object->is_intersected_by(ray, intersections);
      });
    }

    Color shade_hit(const IntersectionState& state, uint8_t depth) const
    {
      Color surface{0, 0, 0};
      Color reflected{0, 0, 0};
      Color refracted{0, 0, 0};

      std::for_each(lights_.begin(), lights_.end(), [this, &surface, &state](const auto& light) {
        surface = surface + lighting(*state.intersection().object(), *light, state.over_point(), state.eye(), state.normal(),
                                     is_shadowed(*light, state.over_point()));
      });

      if (context_.reflections_) {
        reflected = reflect_color(state, depth);
      }
      if (context_.refractions_) {
        refracted = refracted_color(state, depth);
      }

      if (state.intersection().object()->material().reflective() > 0.0 &&
          state.intersection().object()->material().transparency() > 0.0) {
        const auto reflectance = state.schlick();
        return surface + reflected * reflectance + refracted * (1 - reflectance);
      }

      return surface + reflected + refracted;
    }

    bool is_shadowed(const PointLight& light, const Point& point) const
    {
      if (context_.shadows_) {
        auto v = light.position() - point;
        auto distance = v.magnitude();
        auto direction = v.normalize();
        auto ray = Ray{point, direction};
        Intersections intersections;
        intersect(ray, intersections);
        const auto* hit = intersections.hit();
        if (hit && hit->time() < distance) {
          if (has_non_shadowed_object_) {
            return has_shadowed_object(intersections);
          }
          return true;
        }
      }

      return false;
    }

    Color reflect_color(const IntersectionState& state, uint8_t depth) const
    {
      if (depth <= 0 || state.intersection().object()->material().reflective() == Approx(0.0)) {
        return Color{0.0f, 0.0f, 0.0f};
      }

      Intersections intersections;
      auto color = internal_color_at(Ray{state.over_point(), state.reflect()}, intersections, static_cast<uint8_t>(depth - 1));

      return color * state.intersection().object()->material().reflective();
    }

    Color refracted_color(const IntersectionState& state, uint8_t depth) const
    {
      static Color white{1, 1, 1};
      static Color black{0, 0, 0};

      if (depth == 0 || Approx(0.0) == state.intersection().object()->material().transparency()) {
        return black;
      }

      auto n_ratio = state.n1() / state.n2();
      auto cos_i = state.eye().scalarProduct(state.normal());
      auto sin2_t = (n_ratio * n_ratio) * (1 - cos_i * cos_i);

      if (sin2_t > 1.0) {
        return black;
      }

      auto cos_t = sqrt(1.0 - sin2_t);
      auto direction = state.normal() * (n_ratio * cos_i - cos_t) - state.eye() * n_ratio;

      Intersections intersections;
      return internal_color_at(Ray{state.under_point(), direction}, intersections, static_cast<uint8_t>(depth - 1)) *
             state.intersection().object()->material().transparency();
    }

  private:
    bool has_shadowed_object(const Intersections& intersections) const
    {
      auto it =
        std::find_if(intersections.intersections().begin(), intersections.intersections().end(), [](const auto& intersection) {
          return intersection.time() > 0;
        });
      while (it != intersections.intersections().end()) {
        if (it->object()->casts_shadow()) {
          return true;
        }
        ++it;
      }
      return false;
    }

    Color internal_color_at(const Ray& ray, Intersections& intersections, uint8_t depth) const
    {
      intersections.clear();
      intersect(ray, intersections);
      const auto hit = intersections.hit();
      if (hit == nullptr) {
        static Color black{0, 0, 0};
        return black;
      }
      return shade_hit(IntersectionState{*hit, ray, intersections}, depth);
    }

    std::vector<LightPtr> lights_;
    std::vector<ObjectPtr> objects_;
    RenderContext context_;
    bool has_non_shadowed_object_{false};
  };
}
