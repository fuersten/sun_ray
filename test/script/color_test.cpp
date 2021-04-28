//
//  color_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 13.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/shapes/color.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("color construction", "[color]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
  registry.add_meta_class(color_meta_class);

  SECTION("construct default color")
  {
    auto color{color_meta_class->construct()};
    REQUIRE(color);
    CHECK(color->red() == Approx(0));
    CHECK(color->green() == Approx(0));
    CHECK(color->blue() == Approx(0));
  }
  SECTION("construct color")
  {
    auto color{color_meta_class->construct(0.1, 1, 0.5)};
    REQUIRE(color);
    CHECK(color->red() == Approx(0.1));
    CHECK(color->green() == Approx(1));
    CHECK(color->blue() == Approx(0.5));
  }
  SECTION("construct color error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {1.0, 0.9, 0.5, 0.0}));
  }
}

TEST_CASE("color methods", "[color]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
  registry.add_meta_class(color_meta_class);
  auto color{color_meta_class->construct(0.1, 1, 0.5)};

  SECTION("getter")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_get_red", 1));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {color});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0.1));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_get_green", 1));
    res = function_registry.call_function(static_cast<size_t>(idx), {color});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(1));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_get_blue", 1));
    res = function_registry.call_function(static_cast<size_t>(idx), {color});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0.5));
  }
  SECTION("setter")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_set_red", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {color, 0.7});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(color->red() == Approx(0.7));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_set_green", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {color, 0.8});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(color->green() == Approx(0.8));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_set_blue", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {color, 0.9});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(color->blue() == Approx(0.9));
  }
}

TEST_CASE("color stream", "[color]")
{
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();

  SECTION("to string")
  {
    auto color{color_meta_class->construct(0.1, 1, 0.5)};
    CHECK(color->to_string() == "Color r: 0.1 g: 1 b: 0.5");
  }
}
