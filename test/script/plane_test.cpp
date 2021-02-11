//
//  plane_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 04.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/objects/plane.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("plane construction", "[plane]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::PlaneMetaClass>());
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("construct plane")
  {
    auto material{material_meta_class->construct()};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Plane_constructor", 1));
    REQUIRE(idx != -1);
    auto sphere = function_registry.call_function(static_cast<size_t>(idx), {material});
    REQUIRE(sunray::script::is_class(sphere));
  }
  SECTION("construct plane error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Plane_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("plane methods", "[plane]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto plane_meta_class = std::make_shared<sunray::script::PlaneMetaClass>();
  registry.add_meta_class(plane_meta_class);
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  auto material{material_meta_class->construct()};
  auto plane{plane_meta_class->construct(material)};

  SECTION("transform")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Plane_scale", 4));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {plane, 5.0, 5.0, 5.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Plane_shear", 7));
    res = function_registry.call_function(static_cast<size_t>(idx), {plane, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Plane_translate", 4));
    res = function_registry.call_function(static_cast<size_t>(idx), {plane, -1.0, 0.0, 1.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Plane_rotate_x", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {plane, sunray::PI / 4});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Plane_rotate_y", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {plane, sunray::PI / 2});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Plane_rotate_z", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {plane, sunray::PI});
    REQUIRE(sunray::script::is_class(res));
    auto p = std::dynamic_pointer_cast<sunray::script::Plane>(sunray::script::as_class(res));
    REQUIRE(p);
    sunray::Transformation trans;
    trans.scale(5, 5, 5)
      .shear(0, 1, 0, 0, 0, 0)
      .translate(-1, 0, 1)
      .rotate_x(sunray::PI / 4)
      .rotate_y(sunray::PI / 2)
      .rotate_z(sunray::PI);
    CHECK(p->transformation() == trans.matrix());
    auto o = p->shape();
    REQUIRE(o);
    CHECK(std::dynamic_pointer_cast<const sunray::Plane>(o));
  }
}

TEST_CASE("plane stream", "[plane]")
{
  auto sphere_meta_class = std::make_shared<sunray::script::PlaneMetaClass>();
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("to string")
  {
    auto material{material_meta_class->construct()};

    auto sphere{sphere_meta_class->construct(material)};
    CHECK(sphere->to_string() == "Plane");
  }
}
