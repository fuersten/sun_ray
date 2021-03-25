//
//  main.cpp
//  silhouette
//
//  Created by Lars-Christian Fürstenberg on 11.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/canvas_file_writer.h>
#include <sun_ray/feature/ray.h>
#include <sun_ray/feature/sphere.h>
#include <sun_ray/feature/transformation.h>
#include <sun_ray/init.h>

#include <iostream>


int main(int argc, const char* argv[])
{
  try {
    if (argc < 2) {
      std::cerr << "Usage: silhouette <pathname>" << std::endl;
      return -1;
    }

    const auto orange = sunray::Color{1.0f, 0.647f, 0};
    const auto origin = sunray::create_point(0, 0, -5);
    const auto wall_z = 10.0;
    const auto wall_size = 7.0;
    const uint32_t pixels = 500;
    const auto pixel_size = wall_size / pixels;
    const auto half = wall_size / 2.0;

    sunray::Canvas canvas{pixels, pixels};

    sunray::Transformation trans;
    trans.identity();
    // trans.scale(0.5, 1, 1).rotate_z(sunray::PI / 4.0);
    // trans.scale(0.5, 1, 1).shear(1, 0, 0, 0, 0, 0);

    auto sphere = sunray::Sphere::make_sphere(trans.matrix());

    for (uint32_t y = 0; y < pixels; ++y) {
      const auto world_y = half - pixel_size * y;
      for (uint32_t x = 0; x < pixels; ++x) {
        const auto world_x = -half + pixel_size * x;
        const auto position = sunray::create_point(world_x, world_y, wall_z);
        const auto ray = sunray::Ray{origin, (position - origin).normalize()};
        const auto intersections = sunray::intersect(ray, *sphere);
        const auto hit = intersections.hit();
        if (hit != nullptr) {
          canvas.pixel_at(x, y, orange);
        }
      }
    }

    sunray::CanvasFileWriter writer{sunray::ImageFormat::PNG, std::filesystem::path{argv[1]} / "silhouette.png"};
    writer.write(canvas);
  } catch (const std::exception& ex) {
    std::cerr << "exception caught: " << ex.what() << std::endl;
  }

  return 0;
}
