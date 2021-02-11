//
//  main.cpp
//  scene
//
//  Created by Lars-Christian Fürstenberg on 25.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/canvas_file_writer.h>
#include <sun_ray/feature/camera.h>
#include <sun_ray/feature/sphere.h>
#include <sun_ray/feature/transformation.h>
#include <sun_ray/feature/world.h>

#include <chrono>
#include <iostream>


int main(int argc, const char* argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: scene ppm-file-name" << std::endl;
    return -1;
  }
  std::filesystem::path output_file{argv[1]};

  try {
    auto start = std::chrono::steady_clock::now();

    const uint32_t pixels = 2500;

    sunray::World world;
    world.add_light(std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color{1, 1, 1}));
    world.add_light(std::make_shared<sunray::PointLight>(sunray::create_point(10, 10, -10), sunray::Color{0.25f, 0.25f, 0.25f}));

    {
      sunray::Transformation trans;
      trans.scale(10, 0.01, 10);
      auto floor = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{1, 0.9f, 0.9f}, 0.1f, 0.9f, 0, 200.0f, 0, 0, 1},
                                               trans.matrix());
      world.add_object(floor);
    }

    {
      sunray::Transformation trans;
      trans.scale(10, 0.01, 10).rotate_x(sunray::PI2).rotate_y(-sunray::PI / 4.0).translate(0, 0, 5);
      auto left_wall = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{1, 0.9f, 0.9f}, 0.1f, 0.9f, 0, 200.0f, 0, 0, 1},
                                                   trans.matrix());
      world.add_object(left_wall);
    }

    {
      sunray::Transformation trans;
      trans.scale(10, 0.01, 10).rotate_x(sunray::PI2).rotate_y(sunray::PI / 4.0).translate(0, 0, 5);
      auto right_wall = sunray::Sphere::make_sphere(
        sunray::Material{sunray::Color{1, 0.9f, 0.9f}, 0.1f, 0.9f, 0, 200.0f, 0, 0, 1}, trans.matrix());
      world.add_object(right_wall);
    }

    {
      sunray::Transformation trans;
      trans.translate(-0.5, 1, 0.5);
      auto middle = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.1f, 1, 0.5f}, 0.1f, 0.7f, 0.3f, 200.0f, 0, 0, 1},
                                                trans.matrix());
      world.add_object(middle);
    }

    {
      sunray::Transformation trans;
      trans.scale(0.5, 0.5, 0.5).translate(1.5, 0.5, -0.5);
      auto right = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.5f, 1, 0.1f}, 0.1f, 0.7f, 0.3f, 200.0f, 0, 0, 1},
                                               trans.matrix());
      world.add_object(right);
    }

    {
      sunray::Transformation trans;
      trans.scale(0.33, 0.33, 0.33).translate(-1.5, 0.33, -0.75);
      auto left = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{1, 0.8f, 0.1f}, 0.1f, 0.7f, 0.3f, 200.0f, 0, 0, 1},
                                              trans.matrix());
      world.add_object(left);
    }

    auto from = sunray::create_point(0, 1.5, -7);
    auto to = sunray::create_point(0, 1, 0);
    auto up = sunray::create_vector(0, 1, 0);
    sunray::Camera c{pixels, pixels / 2, sunray::PI / 3.0, sunray::view_transformation(from, to, up)};
    auto canvas = c.render(world);

    auto end = std::chrono::steady_clock::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms to process"
              << std::endl;

    start = std::chrono::steady_clock::now();

    sunray::CanvasFileWriter writer{sunray::ImageFormat::PPM6, output_file};
    writer.write(canvas);

    end = std::chrono::steady_clock::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms to save file"
              << std::endl;
  } catch (const std::exception& ex) {
    std::cerr << "exception caught: " << ex.what() << std::endl;
  }

  return 0;
}
