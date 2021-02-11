//
//  camera_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 24.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/camera.h>
#include <sun_ray/feature/sphere.h>
#include <sun_ray/feature/transformation.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("create camera", "[camera]")
{
  SECTION("create camera default transformation")
  {
    sunray::Camera c{160, 140, sunray::PI2};
    CHECK(c.horizontal_size() == 160);
    CHECK(c.vertical_size() == 140);
    CHECK(c.field_of_view() == Approx(sunray::PI2));
    CHECK(c.transformation() == sunray::Matrix44::identity());
  }
  SECTION("create camera custom transformation")
  {
    sunray::Camera c{160, 140, sunray::PI2, sunray::Transformation().translate(0, 0, -8).matrix()};
    CHECK(c.horizontal_size() == 160);
    CHECK(c.vertical_size() == 140);
    CHECK(c.field_of_view() == Approx(sunray::PI2));
    CHECK(c.transformation() == sunray::Transformation().translate(0, 0, -8).matrix());
  }
}

TEST_CASE("stream camera", "[camera]")
{
  SECTION("stream")
  {
    sunray::Camera c{160, 140, sunray::PI2};
    std::stringstream ss;
    ss << c;
    // clang-format off
    const std::string expect = R"(horizontal: 160 vertical: 140 field of view: 1.5708 transformation: | 1 | 0 | -0 | 0 |
| -0 | 1 | -0 | 0 |
| 0 | -0 | 1 | -0 |
| 0 | 0 | -0 | 1 |
)";
    // clang-format on
    CHECK(expect == ss.str());
  }
}

TEST_CASE("pixel size", "[camera]")
{
  SECTION("pixel size horizontal canvas")
  {
    sunray::Camera c{200, 125, sunray::PI2};
    CHECK(c.pixel_size() == Approx(0.01));
  }
  SECTION("pixel size vertical canvas")
  {
    sunray::Camera c{125, 200, sunray::PI2};
    CHECK(c.pixel_size() == Approx(0.01));
  }
}

TEST_CASE("ray through camera", "[camera]")
{
  SECTION("center of canvas")
  {
    sunray::Camera c{201, 101, sunray::PI2};
    auto ray = c.ray_for_pixel(100, 50);
    CHECK(ray == sunray::Ray(sunray::create_point(0, 0, 0), sunray::create_vector(0, 0, -1)));
  }
  SECTION("corner of canvas")
  {
    sunray::Camera c{201, 101, sunray::PI2};
    auto ray = c.ray_for_pixel(0, 0);
    CHECK(ray == sunray::Ray(sunray::create_point(0, 0, 0), sunray::create_vector(0.66519, 0.33259, -0.66851)));
  }
  SECTION("center of canvas with transformation")
  {
    sunray::Camera c{201, 101, sunray::PI2, sunray::Transformation().translate(0, -2, 5).rotate_y(sunray::PI / 4.0).matrix()};
    auto ray = c.ray_for_pixel(100, 50);
    CHECK(ray ==
          sunray::Ray(sunray::create_point(0, 2, -5), sunray::create_vector(sunray::sqrt(2) / 2, 0, -sunray::sqrt(2) / 2)));
  }
}

TEST_CASE("render world", "[camera]")
{
  SECTION("center of canvas with transformation")
  {
    sunray::World world;
    auto light = std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color(1, 1, 1));
    world.add_light(light);

    auto s =
      sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 0.1f, 0.7f, 0.2f, 200.0f, 0.0f, 0.0f, 1.0f});
    world.add_object(s);

    s = sunray::Sphere::make_sphere(sunray::Transformation().scale(0.5, 0.5, 0.5).matrix());
    world.add_object(s);

    auto from = sunray::create_point(0, 0, -5);
    auto to = sunray::create_point(0, 0, 0);
    auto up = sunray::create_vector(0, 1, 0);
    sunray::Camera c{11, 11, sunray::PI2, sunray::view_transformation(from, to, up)};
    auto canvas = c.render(world);
    CHECK(canvas.pixel_at(5, 5) == sunray::Color{0.38066f, 0.47583f, 0.2855f});
  }
}
