//
//  main.cpp
//  sphere
//
//  Created by Lars-Christian Fürstenberg on 12.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/canvas_file_writer.h>
#include <sun_ray/feature/ray.h>
#include <sun_ray/feature/sphere.h>
#include <sun_ray/feature/transformation.h>
#include <sun_ray/feature/world.h>

#include <chrono>
#include <iostream>


int main(int argc, const char* argv[])
{
  (void)argc;
  (void)argv;

  try {
    auto start = std::chrono::steady_clock::now();

    const auto origin = sunray::create_point(0, 0, -5);
    const auto wall_z = 10.0;
    const auto wall_size = 7.0;
    const uint32_t pixels = 1000;
    const auto pixel_size = wall_size / pixels;
    const auto half = wall_size / 2.0;

    sunray::Canvas canvas{pixels, pixels};

    sunray::World world;
    world.add_light(std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color{1, 0.84f, 0}));
    world.add_light(std::make_shared<sunray::PointLight>(sunray::create_point(10, 10, -10), sunray::Color{0.5f, 0.5f, 0.5f}));

    sunray::Transformation trans;
    trans.identity().scale(10, 0.01, 10).translate(0, -1, 0);
    auto sphere = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0, 0.0f, 1.0f},
                                              trans.matrix());
    world.add_object(sphere);

    sunray::Transformation trans1;
    trans1.identity().scale(0.5, 0.5, 0.5);
    sphere = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{1, 0, 0}, 0.1f, 0.9f, 0.9f, 200.0f, 0, 0.0f, 1.0f},
                                         trans1.matrix());
    world.add_object(sphere);

    sunray::Intersections intersections;
    for (uint32_t y = 0; y < pixels; ++y) {
      const auto world_y = half - pixel_size * y;
      for (uint32_t x = 0; x < pixels; ++x) {
        const auto world_x = -half + pixel_size * x;
        const auto position = sunray::create_point(world_x, world_y, wall_z);
        const auto ray = sunray::Ray{origin, (position - origin).normalize()};

        canvas.pixel_at(x, y, world.color_at(ray, intersections));
      }
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms to process"
              << std::endl;

    start = std::chrono::steady_clock::now();

    sunray::CanvasFileWriter writer{sunray::ImageFormat::PPM3, "/Users/lcf/temp/sphere.ppm"};
    writer.write(canvas);

    end = std::chrono::steady_clock::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms to save file"
              << std::endl;
  } catch (const std::exception& ex) {
    std::cerr << "exception caught: " << ex.what() << std::endl;
  }

  return 0;
}
