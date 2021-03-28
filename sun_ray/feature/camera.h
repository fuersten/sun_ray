//
//  camera.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 24.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/canvas.h>
#include <sun_ray/feature/ray.h>
#include <sun_ray/feature/world.h>

#include <thread>


namespace sunray
{
  class Camera
  {
  public:
    Camera(uint32_t h_size, uint32_t v_size, double field_of_view)
    : horizontal_size_{h_size}
    , vertical_size_{v_size}
    , field_of_view_{field_of_view}
    {
      calculate_pixel_size();
    }

    Camera(uint32_t h_size, uint32_t v_size, double field_of_view, Matrix44 transformation)
    : horizontal_size_{h_size}
    , vertical_size_{v_size}
    , field_of_view_{field_of_view}
    , transformation_{transformation.inverse()}
    {
      calculate_pixel_size();
    }

    ~Camera() = default;

    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;

    Ray ray_for_pixel(uint32_t x, uint32_t y) const
    {
      auto x_offset = (x + 0.5) * pixel_size_;
      auto y_offset = (y + 0.5) * pixel_size_;
      auto world_p = create_point(half_width_ - x_offset, half_height_ - y_offset, -1);

      static auto s_origin = create_point(0, 0, 0);
      auto pixel = transformation_ * world_p;
      auto origin = transformation_ * s_origin;
      auto direction = (pixel - origin).normalize();

      return Ray{origin, direction};
    }

    Canvas render(const World& world) const
    {
      Canvas canvas{horizontal_size_, vertical_size_};

      struct RenderPart {
        uint32_t from_;
        uint32_t to_;
      };

      uint32_t nr_parts = std::thread::hardware_concurrency();
      uint32_t part_size = vertical_size_ / nr_parts;

      if (part_size == 0) {
        nr_parts = 1;
        part_size = vertical_size_;
      }

      std::vector<RenderPart> parts;
      uint32_t vertical_lines{0};
      for (uint32_t n = 0; n < nr_parts; ++n) {
        parts.emplace_back(RenderPart{part_size * n, part_size * (n + 1)});
        vertical_lines += parts.back().to_ - parts.back().from_;
      }

      if (vertical_lines < vertical_size_) {
        parts.emplace_back(RenderPart{parts.back().from_ + 1, vertical_size_});
      }

      std::vector<std::thread> threads;
      for (const auto& part : parts) {
        threads.emplace_back(std::thread{[&]() {
          render(canvas, world, part.from_, part.to_);
        }});
      }

      for (auto& thread : threads) {
        if (thread.joinable()) {
          thread.join();
        }
      }

      return canvas;
    }

    inline uint32_t horizontal_size() const
    {
      return horizontal_size_;
    }

    inline uint32_t vertical_size() const
    {
      return vertical_size_;
    }

    inline double field_of_view() const
    {
      return field_of_view_;
    }

    inline Matrix44 transformation() const
    {
      return transformation_.inverse();
    }

    inline double pixel_size() const
    {
      return pixel_size_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Camera& camera)
    {
      stream << "horizontal: " << camera.horizontal_size() << " vertical: " << camera.vertical_size()
             << " field of view: " << camera.field_of_view() << " transformation: " << camera.transformation();
      return stream;
    }

  private:
    void render(Canvas& canvas, const World& world, uint32_t from, uint32_t to) const
    {
      sunray::Intersections intersections;
      for (uint32_t y = from; y < to; ++y) {
        for (uint32_t x = 0; x < horizontal_size_ - 1; ++x) {
          const auto ray = ray_for_pixel(x, y);
          canvas.pixel_at(x, y, world.color_at(ray, intersections));
        }
      }
    }

    void calculate_pixel_size()
    {
      double half_view = ::tan(field_of_view_ / 2.0);
      auto aspect_ratio = static_cast<double>(horizontal_size_) / vertical_size_;
      if (aspect_ratio >= 1.0) {
        half_width_ = half_view;
        half_height_ = half_view / aspect_ratio;
      } else {
        half_width_ = half_view * aspect_ratio;
        half_height_ = half_view;
      }
      pixel_size_ = (half_width_ * 2.0) / horizontal_size_;
    }

    uint32_t horizontal_size_;
    uint32_t vertical_size_;
    double field_of_view_;
    double half_width_;
    double half_height_;
    double pixel_size_;
    Matrix44 transformation_{Matrix44::identity().inverse()};
  };
}
