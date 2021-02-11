//
//  meta_class_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 08.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/meta_class.h>
#include <sun_ray/script/objects/color.h>
#include <sun_ray/script/objects/point.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("meta class", "[meta class]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
  registry.add_meta_class(color_meta_class);

  SECTION("find meta class")
  {
    auto meta_class = registry.find("Color");
    REQUIRE(meta_class != nullptr);
    CHECK(meta_class->name() == "Color");
  }
  SECTION("call class")
  {
    auto color{color_meta_class->construct()};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_set_red", 2));
    REQUIRE(idx != -1);
    auto res = function_registry.call_function(static_cast<size_t>(idx), {color, 1.68});
    CHECK(sunray::script::is_double(res));
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_get_red", 1));
    res = function_registry.call_function(static_cast<size_t>(idx), {color});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(1.68));
  }
  SECTION("add known meta class")
  {
    CHECK_FALSE(registry.add_meta_class(color_meta_class));
  }
  SECTION("cast class")
  {
    sunray::script::MutableClassPtr cl = color_meta_class->construct();
    CHECK_NOTHROW(sunray::script::cast_object<sunray::script::Color, sunray::script::ColorMetaClass>(cl, "color"));
    CHECK_NOTHROW(sunray::script::cast_object<sunray::script::Color>(cl, "Material", "color"));
  }
  SECTION("cast class error")
  {
    sunray::script::MutableClassPtr cl = color_meta_class->construct();
    auto func1 = [&cl]() {
      sunray::script::cast_object<sunray::script::Point, sunray::script::PointMetaClass>(cl, "point");
    };
    CHECK_THROWS_WITH(func1(), "Point add wrong parameter for point. Should be Point but is Color.");
    auto func2 = [&cl]() {
      sunray::script::cast_object<sunray::script::Point>(cl, "Light", "point");
    };
    CHECK_THROWS_WITH(func2(), "Light add wrong parameter for point. Should be Light but is Color.");
  }
}

TEST_CASE("call constructor", "[meta class]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
  registry.add_meta_class(color_meta_class);

  SECTION("construct default color")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_constructor", 1));
    REQUIRE(idx != -1);
    auto color = function_registry.call_function(static_cast<size_t>(idx), {});
    REQUIRE(sunray::script::is_class(color));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_get_red", 1));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {color});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0.0));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_get_green", 1));
    res = function_registry.call_function(static_cast<size_t>(idx), {color});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0.0));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_get_blue", 1));
    res = function_registry.call_function(static_cast<size_t>(idx), {color});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0.0));
  }
  SECTION("construct color")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_constructor", 1));
    REQUIRE(idx != -1);
    auto color = function_registry.call_function(static_cast<size_t>(idx), {0.1, 1.0, 0.5});
    REQUIRE(sunray::script::is_class(color));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Color_get_red", 1));
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
  SECTION("get const class")
  {
    sunray::script::ClassPtr clazz{color_meta_class->construct(1, 0.5, 0.9)};
    const sunray::script::Color* color = color_meta_class->get_class(clazz);
    CHECK(color);
  }
  SECTION("get class")
  {
    sunray::script::MutableClassPtr clazz{color_meta_class->construct(1, 0.5, 0.9)};
    sunray::script::Color* color = color_meta_class->get_class(clazz);
    CHECK(color);
  }
}
