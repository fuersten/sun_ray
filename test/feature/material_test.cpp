//
//  material_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 12.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/material.h>
#include <sun_ray/feature/sphere.h>
#include <sun_ray/feature/stripe_pattern.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("create materials", "[materials]")
{
  SECTION("create default material")
  {
    sunray::Material material;
    CHECK(material.color() == sunray::Color(1, 1, 1));
    CHECK(material.ambient() == Approx(0.1));
    CHECK(material.diffuse() == Approx(0.9));
    CHECK(material.specular() == Approx(0.9));
    CHECK(material.shininess() == Approx(200.0));
  }
  SECTION("create material")
  {
    sunray::Material material{sunray::Color(1, 0, 0), 0.2f, 0.7f, 0.8f, 100.0f, 0.1f, 0.5f, 1.25f};
    CHECK(material.color() == sunray::Color(1, 0, 0));
    CHECK(material.ambient() == Approx(0.2));
    CHECK(material.diffuse() == Approx(0.7));
    CHECK(material.specular() == Approx(0.8));
    CHECK(material.shininess() == Approx(100.0));
    CHECK(material.reflective() == Approx(0.1));
    CHECK(material.transparency() == Approx(0.5));
    CHECK(material.refractive_index() == Approx(1.25));
  }
}

TEST_CASE("stream material", "[materials]")
{
  SECTION("stream")
  {
    sunray::Material material{sunray::Color(1, 1, 1), 0.1f, 0.9f, 0.8f, 200.0f, 0.5f, 0.0f, 1.0f};
    std::stringstream ss;
    ss << material;
    CHECK("color: red: 1 green: 1 blue: 1 ambient: 0.1 diffuse: 0.9 specular: 0.8 shininess: 200 reflective: 0.5 transparency: 0 "
          "refractive_index: 1" == ss.str());
  }
}

TEST_CASE("lighting", "[materials]")
{
  sunray::Material material;
  auto position = sunray::create_point(0, 0, 0);
  auto sphere = sunray::Sphere::make_sphere(material);

  SECTION("lighting with eye between light and surface")
  {
    auto eyev = sunray::create_vector(0, 0, -1);
    auto normalv = sunray::create_vector(0, 0, -1);
    auto light = sunray::PointLight(sunray::create_point(0, 0, -10), sunray::Color{1, 1, 1});
    auto result_color = sunray::lighting(*sphere, light, position, eyev, normalv, true);
    CHECK(result_color == sunray::Color(0.1f, 0.1f, 0.1f));
  }
  SECTION("lighting with eye between light and surface, eye offset 45°")
  {
    auto eyev = sunray::create_vector(0, sunray::sqrt(2) / 2, -sunray::sqrt(2) / 2);
    auto normalv = sunray::create_vector(0, 0, -1);
    auto light = sunray::PointLight(sunray::create_point(0, 0, -10), sunray::Color{1, 1, 1});
    auto result_color = sunray::lighting(*sphere, light, position, eyev, normalv, false);
    CHECK(result_color == sunray::Color(1, 1, 1));
  }
  SECTION("lighting with eye opposite surface, light offset 45°")
  {
    auto eyev = sunray::create_vector(0, 0, -1);
    auto normalv = sunray::create_vector(0, 0, -1);
    auto light = sunray::PointLight(sunray::create_point(0, 10, -10), sunray::Color{1, 1, 1});
    auto result_color = sunray::lighting(*sphere, light, position, eyev, normalv, false);
    CHECK(result_color == sunray::Color(0.7364f, 0.7364f, 0.7364f));
  }
  SECTION("lighting with eye in the path of the reflection vector")
  {
    auto eyev = sunray::create_vector(0, -sunray::sqrt(2) / 2, -sunray::sqrt(2) / 2);
    auto normalv = sunray::create_vector(0, 0, -1);
    auto light = sunray::PointLight(sunray::create_point(0, 10, -10), sunray::Color{1, 1, 1});
    auto result_color = sunray::lighting(*sphere, light, position, eyev, normalv, false);
    CHECK(result_color == sunray::Color(1.6364f, 1.6364f, 1.6364f));
  }
  SECTION("lighting with light behind the surface")
  {
    auto eyev = sunray::create_vector(0, 0, -1);
    auto normalv = sunray::create_vector(0, 0, -1);
    auto light = sunray::PointLight(sunray::create_point(0, 0, 10), sunray::Color{1, 1, 1});
    auto result_color = sunray::lighting(*sphere, light, position, eyev, normalv, false);
    CHECK(result_color == sunray::Color(0.1f, 0.1f, 0.1f));
  }
}

TEST_CASE("lighting with patterns", "[materials]")
{
  sunray::Material material{sunray::Color(1, 1, 1), 1, 0, 0, 200.0f, 0.0f, 0.0f, 1.0f};
  auto pattern = std::make_shared<sunray::StripePattern>(sunray::Color{1, 1, 1}, sunray::Color{0, 0, 0});
  material.pattern(pattern);
  auto sphere = sunray::Sphere::make_sphere(material);

  SECTION("lighting with a pattern applied")
  {
    auto eyev = sunray::create_vector(0, 0, -1);
    auto normalv = sunray::create_vector(0, 0, -1);
    auto light = sunray::PointLight(sunray::create_point(0, 0, -10), sunray::Color{1, 1, 1});
    auto result_color = sunray::lighting(*sphere, light, sunray::create_point(0.9, 0, 0), eyev, normalv, false);
    CHECK(result_color == sunray::Color(1, 1, 1));
    result_color = sunray::lighting(*sphere, light, sunray::create_point(1.1, 0, 0), eyev, normalv, false);
    CHECK(result_color == sunray::Color(0, 0, 0));
  }
}
