//
//  canvas_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 11.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/objects/canvas.h>

#include <sstream>

#include <catch2/catch.hpp>

using namespace std::string_literals;


TEST_CASE("canvas construction", "[canvas]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto canva_meta_class = std::make_shared<sunray::script::CanvasMetaClass>();
  registry.add_meta_class(canva_meta_class);

  SECTION("construct canvas")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Canvas_constructor", 1));
    REQUIRE(idx != -1);
    auto canvas = function_registry.call_function(static_cast<size_t>(idx), {250.0, 500.0});
    REQUIRE(sunray::script::is_class(canvas));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Canvas_get_width", 1));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {canvas});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(250));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Canvas_get_height", 1));
    res = function_registry.call_function(static_cast<size_t>(idx), {canvas});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(500));
  }
  SECTION("construct canvas with canvas")
  {
    auto canvas = std::make_shared<sunray::script::Canvas>(canva_meta_class, sunray::Canvas{2000, 1000});

    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Canvas_get_width", 1));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {canvas});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(2000));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Canvas_get_height", 1));
    res = function_registry.call_function(static_cast<size_t>(idx), {canvas});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(1000));
  }
  SECTION("construct canvas error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Canvas_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {250.0, 500.0, 750.0}));
  }
}

TEST_CASE("canvas set pixel", "[canvas]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto canva_meta_class = std::make_shared<sunray::script::CanvasMetaClass>();
  registry.add_meta_class(canva_meta_class);
  registry.add_meta_class(std::make_shared<sunray::script::ColorMetaClass>());
  auto canvas{canva_meta_class->construct(500, 250)};

  SECTION("set pixel")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_constructor", 1));
    REQUIRE(idx != -1);
    auto color = function_registry.call_function(static_cast<size_t>(idx), {0.1, 1.0, 0.5});
    REQUIRE(sunray::script::is_class(color));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Canvas_set_pixel", 4));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {canvas, 10.0, 100.0, color});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
  }
  SECTION("set pixel error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Canvas_set_pixel", 4));
    CHECK_THROWS(
      function_registry.call_function(static_cast<size_t>(idx), {canvas, 10.0, 100.0, sunray::script::MutableClassPtr{}}));
  }
}

TEST_CASE("canvas write", "[canvas]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto canva_meta_class = std::make_shared<sunray::script::CanvasMetaClass>();
  registry.add_meta_class(canva_meta_class);
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
  registry.add_meta_class(color_meta_class);
  auto canvas{canva_meta_class->construct(11, 11)};

  SECTION("write")
  {
    auto color = color_meta_class->construct(1.0, 0.9, 0.5);
    canvas->set_pixel(5, 5, *color);

    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Canvas_write", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {canvas, "canvas"s});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
  }
}

TEST_CASE("canvas stream", "[canvas]")
{
  auto canva_meta_class = std::make_shared<sunray::script::CanvasMetaClass>();

  SECTION("to string")
  {
    auto canvas{canva_meta_class->construct(500, 250)};
    CHECK(canvas->to_string() == "Canvas w: 500 h: 250");
  }
}
