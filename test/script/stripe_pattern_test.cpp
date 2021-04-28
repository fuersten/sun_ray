//
//  stripe_pattern_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 14.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/shapes/stripe_pattern.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("stripe pattern construction", "[stripe pattern]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::StripePatternMetaClass>());
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();

  SECTION("construct pattern")
  {
    auto color_a{color_meta_class->construct(0.1, 1, 0.5)};
    auto color_b{color_meta_class->construct(0.1, 1, 0.5)};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("StripePattern_constructor", 1));
    REQUIRE(idx != -1);
    auto pattern = function_registry.call_function(static_cast<size_t>(idx), {color_a, color_b});
    REQUIRE(sunray::script::is_class(pattern));
    REQUIRE(std::dynamic_pointer_cast<sunray::script::StripePattern>(sunray::script::as_class(pattern))->pattern());
  }
  SECTION("construct pattern error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("StripePattern_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("stripe pattern methods", "[stripe pattern]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto pattern_meta_class = std::make_shared<sunray::script::StripePatternMetaClass>();
  registry.add_meta_class(pattern_meta_class);
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
  auto color_a{color_meta_class->construct(0.1, 1, 0.5)};
  auto color_b{color_meta_class->construct(0.1, 1, 0.5)};
  auto pattern{pattern_meta_class->construct(color_a, color_b)};

  SECTION("transform")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("StripePattern_scale", 4));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {pattern, 5.0, 5.0, 5.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("StripePattern_shear", 7));
    res = function_registry.call_function(static_cast<size_t>(idx), {pattern, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("StripePattern_translate", 4));
    res = function_registry.call_function(static_cast<size_t>(idx), {pattern, -1.0, 0.0, 1.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("StripePattern_rotate_x", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {pattern, sunray::PI / 4});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("StripePattern_rotate_y", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {pattern, sunray::PI / 2});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("StripePattern_rotate_z", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {pattern, sunray::PI});
    REQUIRE(sunray::script::is_class(res));
    auto p = std::dynamic_pointer_cast<sunray::script::StripePattern>(sunray::script::as_class(res));
    REQUIRE(p);
    sunray::Transformation trans;
    trans.scale(5, 5, 5)
      .shear(0, 1, 0, 0, 0, 0)
      .translate(-1, 0, 1)
      .rotate_x(sunray::PI / 4)
      .rotate_y(sunray::PI / 2)
      .rotate_z(sunray::PI);
    CHECK(p->transformation() == trans.matrix());
  }
}

TEST_CASE("stripe pattern stream", "[stripe pattern]")
{
  auto pattern_meta_class = std::make_shared<sunray::script::StripePatternMetaClass>();
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
  auto color_a{color_meta_class->construct(0.1, 1, 0.5)};
  auto color_b{color_meta_class->construct(0.1, 1, 0.5)};
  auto pattern{pattern_meta_class->construct(color_a, color_b)};

  SECTION("to string")
  {
    CHECK(pattern->to_string() == "StripePattern");
  }
}
