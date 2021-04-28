//
//  light_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 15.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/shapes/light.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("light construction", "[light]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::LightMetaClass>());
  auto point_meta_class = std::make_shared<sunray::script::PointMetaClass>();
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();

  SECTION("construct light")
  {
    auto point{point_meta_class->construct(-2, 3, 1)};
    auto color{color_meta_class->construct(0.1, 1, 0.5)};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Light_constructor", 1));
    REQUIRE(idx != -1);
    auto light = function_registry.call_function(static_cast<size_t>(idx), {point, color});
    REQUIRE(sunray::script::is_class(light));
    CHECK(std::dynamic_pointer_cast<sunray::script::Light>(sunray::script::as_class(light))->light()->intensity() ==
          sunray::Color{0.1f, 1.0f, 0.5f});
  }
  SECTION("construct default light")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Light_constructor", 1));
    REQUIRE(idx != -1);
    auto light = function_registry.call_function(static_cast<size_t>(idx), {});
    REQUIRE(sunray::script::is_class(light));
    CHECK(std::dynamic_pointer_cast<sunray::script::Light>(sunray::script::as_class(light))->light()->intensity() ==
          sunray::Color{1.0f, 1.0f, 1.0f});
  }
  SECTION("construct light error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Light_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("light properties", "[light]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto light_meta_class = std::make_shared<sunray::script::LightMetaClass>();
  registry.add_meta_class(light_meta_class);
  auto point_meta_class = std::make_shared<sunray::script::PointMetaClass>();
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
  auto light{light_meta_class->construct()};

  SECTION("set position")
  {
    auto point{point_meta_class->construct(-2, 3, 1)};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Light_set_position", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {light, point});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(light->light()->position().x() == Approx(-2));
    CHECK(light->light()->position().y() == Approx(3));
    CHECK(light->light()->position().z() == Approx(1));
  }
  SECTION("set intensity")
  {
    auto color{color_meta_class->construct(0.1, 1, 0.5)};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Light_set_intensity", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {light, color});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(light->light()->intensity().red() == Approx(0.1));
    CHECK(light->light()->intensity().green() == Approx(1));
    CHECK(light->light()->intensity().blue() == Approx(0.5));
  }
}

TEST_CASE("light stream", "[light]")
{
  auto light_meta_class = std::make_shared<sunray::script::LightMetaClass>();
  auto point_meta_class = std::make_shared<sunray::script::PointMetaClass>();
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();

  SECTION("to string")
  {
    auto point{point_meta_class->construct(-2, 3, 1)};
    auto color{color_meta_class->construct(0.1, 1, 0.5)};

    auto light{light_meta_class->construct(point, color)};
    CHECK(light->to_string() == "Light p: {x: -2 y: 3 z: 1 w: 1} i: {red: 0.1 green: 1 blue: 0.5}");
  }
}
