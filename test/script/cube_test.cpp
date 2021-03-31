//
//  cube_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 31.03.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/objects/cube.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("cube construction", "[cube]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::CubeMetaClass>());
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("construct cube")
  {
    auto material{material_meta_class->construct()};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cube_constructor", 1));
    REQUIRE(idx != -1);
    auto cube = function_registry.call_function(static_cast<size_t>(idx), {material});
    REQUIRE(sunray::script::is_class(cube));
  }
  SECTION("construct cube error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cube_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("cube methods", "[cube]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto cube_meta_class = std::make_shared<sunray::script::CubeMetaClass>();
  registry.add_meta_class(cube_meta_class);
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  auto material{material_meta_class->construct()};
  auto cube{cube_meta_class->construct(material)};

  SECTION("transform")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cube_scale", 4));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {cube, 5.0, 5.0, 5.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cube_shear", 7));
    res = function_registry.call_function(static_cast<size_t>(idx), {cube, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cube_translate", 4));
    res = function_registry.call_function(static_cast<size_t>(idx), {cube, -1.0, 0.0, 1.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cube_rotate_x", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {cube, sunray::PI / 4});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cube_rotate_y", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {cube, sunray::PI / 2});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cube_rotate_z", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {cube, sunray::PI});
    REQUIRE(sunray::script::is_class(res));
    auto p = std::dynamic_pointer_cast<sunray::script::Cube>(sunray::script::as_class(res));
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
    CHECK(std::dynamic_pointer_cast<const sunray::Cube>(o));
  }
}

TEST_CASE("cube stream", "[cube]")
{
  auto cube_meta_class = std::make_shared<sunray::script::CubeMetaClass>();
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("to string")
  {
    auto material{material_meta_class->construct()};

    auto cube{cube_meta_class->construct(material)};
    CHECK(cube->to_string() == "Cube");
  }
}
