//
//  world_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 18.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/shapes/world.h>

#include <sstream>

#include <catch2/catch.hpp>

using namespace std::string_literals;


TEST_CASE("world construction", "[world]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::WorldMetaClass>());

  SECTION("construct canvas")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("World_constructor", 1));
    REQUIRE(idx != -1);
    auto world = function_registry.call_function(static_cast<size_t>(idx), {});
    REQUIRE(sunray::script::is_class(world));
  }
  SECTION("construct world error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("World_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {250.0, 500.0, 750.0}));
  }
}

TEST_CASE("world methods", "[world]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto world_meta_class = std::make_shared<sunray::script::WorldMetaClass>();
  registry.add_meta_class(world_meta_class);
  auto light_meta_class = std::make_shared<sunray::script::LightMetaClass>();
  auto point_meta_class = std::make_shared<sunray::script::PointMetaClass>();
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
  auto sphere_meta_class = std::make_shared<sunray::script::SphereMetaClass>();
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  auto world = world_meta_class->construct();
  auto material{material_meta_class->construct()};
  auto sphere{sphere_meta_class->construct(material)};
  auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("World_add", 2));

  SECTION("add objects")
  {
    auto point{point_meta_class->construct(-2, 3, 1)};
    auto color{color_meta_class->construct(0.1, 1, 0.5)};
    auto light{light_meta_class->construct(point, color)};

    REQUIRE(idx != -1);
    auto res = function_registry.call_function(static_cast<size_t>(idx), {world, sphere});
    REQUIRE(sunray::script::is_class(res));
  }
  SECTION("add light")
  {
    auto point{point_meta_class->construct(-2, 3, 1)};
    auto color{color_meta_class->construct(0.1, 1, 0.5)};
    auto light{light_meta_class->construct(point, color)};

    REQUIRE(idx != -1);
    auto res = function_registry.call_function(static_cast<size_t>(idx), {world, light});
    REQUIRE(sunray::script::is_class(res));
  }
  SECTION("wrong object")
  {
    REQUIRE(idx != -1);
    CHECK_THROWS_WITH(function_registry.call_function(static_cast<size_t>(idx), {world, material}),
                      "Function 'World_add' error: World add has to be called with light or a shape");
  }
}

TEST_CASE("world properties", "[world]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto world_meta_class = std::make_shared<sunray::script::WorldMetaClass>();
  registry.add_meta_class(world_meta_class);
  auto world = world_meta_class->construct();

  SECTION("set shadows")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("World_set_shadows", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {world, false});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK_FALSE(world->world().context().shadows_);
  }
  SECTION("set reflections")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("World_set_reflections", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {world, false});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK_FALSE(world->world().context().reflections_);
  }
  SECTION("set refractions")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("World_set_refractions", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {world, false});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK_FALSE(world->world().context().refractions_);
  }
}

TEST_CASE("world stream", "[world]")
{
  auto world_meta_class = std::make_shared<sunray::script::WorldMetaClass>();

  SECTION("to string")
  {
    auto world{world_meta_class->construct()};
    CHECK(world->to_string() == "World");
  }
}
