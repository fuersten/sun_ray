//
//  material_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 16.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/shapes/material.h>
#include <sun_ray/script/shapes/ring_pattern.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("material construction", "[material]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  registry.add_meta_class(material_meta_class);

  SECTION("construct material")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_constructor", 1));
    REQUIRE(idx != -1);
    auto material = function_registry.call_function(static_cast<size_t>(idx), {});
    REQUIRE(sunray::script::is_class(material));
  }
  SECTION("copy construct material")
  {
    auto material{material_meta_class->construct()};
    material->ambient(0.2f);
    material->specular(0.8f);
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_constructor", 1));
    REQUIRE(idx != -1);
    auto cmaterial = function_registry.call_function(static_cast<size_t>(idx), {material});
    REQUIRE(sunray::script::is_class(cmaterial));
    auto m = std::dynamic_pointer_cast<sunray::script::Material>(sunray::script::as_class(cmaterial));
    REQUIRE(m != nullptr);
  }
  SECTION("construct material error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("material methods", "[material]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  registry.add_meta_class(material_meta_class);
  auto material{material_meta_class->construct()};
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();

  SECTION("set color")
  {
    auto color{color_meta_class->construct(0.1, 1, 0.5)};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_set_color", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {material, color});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(material->color().red() == Approx(0.1));
    CHECK(material->color().green() == Approx(1));
    CHECK(material->color().blue() == Approx(0.5));
  }
  SECTION("set ambient")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_set_ambient", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {material, 0.2});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(material->ambient() == Approx(0.2));
  }
  SECTION("set diffuse")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_set_diffuse", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {material, 0.7});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(material->diffuse() == Approx(0.7));
  }
  SECTION("set specular")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_set_specular", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {material, 0.8});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(material->specular() == Approx(0.8));
  }
  SECTION("set shininess")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_set_shininess", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {material, 100.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(material->shininess() == Approx(100.0));
  }
  SECTION("set reflective")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_set_reflective", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {material, 1.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(material->reflective() == Approx(1.0));
  }
  SECTION("set transparency")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_set_transparency", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {material, 0.5});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(material->transparency() == Approx(0.5));
  }
  SECTION("set refractive index")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_set_refractive_index", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {material, 0.5});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(material->refractive_index() == Approx(0.5));
  }
  SECTION("set pattern")
  {
    auto pattern_meta_class = std::make_shared<sunray::script::RingPatternMetaClass>();
    auto color_a{color_meta_class->construct(0.1, 1, 0.5)};
    auto color_b{color_meta_class->construct(0.1, 1, 0.5)};
    auto pattern{pattern_meta_class->construct(color_a, color_b)};
    CHECK_FALSE(material->has_pattern());
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Material_set_pattern", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {material, pattern});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(material->has_pattern());
  }
}

TEST_CASE("material stream", "[material]")
{
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("to string")
  {
    auto material{material_meta_class->construct()};
    CHECK(material->to_string() == "Material c: {red: 1 green: 1 blue: 1} a: 0.1 d: 0.9 s: 0.9 sh: 200 r: 0 t: 0 ri: 1");
  }
}
