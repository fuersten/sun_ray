//
//  sphere_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 16.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/shapes/sphere.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("sphere construction", "[sphere]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::SphereMetaClass>());
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("construct sphere")
  {
    auto material{material_meta_class->construct()};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Sphere_constructor", 1));
    REQUIRE(idx != -1);
    auto sphere = function_registry.call_function(static_cast<size_t>(idx), {material});
    REQUIRE(sunray::script::is_class(sphere));
  }
  SECTION("construct sphere error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Sphere_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("sphere methods", "[sphere]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto sphere_meta_class = std::make_shared<sunray::script::SphereMetaClass>();
  registry.add_meta_class(sphere_meta_class);
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  auto material{material_meta_class->construct()};
  auto sphere{sphere_meta_class->construct(material)};

  SECTION("set casts_shadow")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Sphere_set_casts_shadow", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {sphere, false});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
  }
  SECTION("transform")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Sphere_scale", 4));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {sphere, 5.0, 5.0, 5.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Sphere_shear", 7));
    res = function_registry.call_function(static_cast<size_t>(idx), {sphere, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Sphere_translate", 4));
    res = function_registry.call_function(static_cast<size_t>(idx), {sphere, -1.0, 0.0, 1.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Sphere_rotate_x", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {sphere, sunray::PI / 4});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Sphere_rotate_y", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {sphere, sunray::PI / 2});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Sphere_rotate_z", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {sphere, sunray::PI});
    REQUIRE(sunray::script::is_class(res));
    auto p = std::dynamic_pointer_cast<sunray::script::Sphere>(sunray::script::as_class(res));
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

TEST_CASE("sphere stream", "[sphere]")
{
  auto sphere_meta_class = std::make_shared<sunray::script::SphereMetaClass>();
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("to string")
  {
    auto material{material_meta_class->construct()};

    auto sphere{sphere_meta_class->construct(material)};
    CHECK(sphere->to_string() == "Sphere");
  }
}
