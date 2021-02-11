//
//  camera_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 18.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/objects/camera.h>

#include <sstream>

#include <catch2/catch.hpp>

using namespace std::string_literals;


TEST_CASE("camera construction", "[camera]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::CameraMetaClass>());

  SECTION("construct camera")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Camera_constructor", 1));
    REQUIRE(idx != -1);
    auto camera = function_registry.call_function(static_cast<size_t>(idx), {});
    REQUIRE(sunray::script::is_class(camera));
  }
  SECTION("construct camera error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Camera_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {250.0, 500.0, 750.0}));
  }
}

TEST_CASE("camera methods", "[camera]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto camera_meta_class = std::make_shared<sunray::script::CameraMetaClass>();
  registry.add_meta_class(camera_meta_class);
  auto point_meta_class = std::make_shared<sunray::script::PointMetaClass>();
  auto vector_meta_class = std::make_shared<sunray::script::VectorMetaClass>();
  auto camera{camera_meta_class->construct()};

  SECTION("set from")
  {
    auto point{point_meta_class->construct(1, 2, 3)};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Camera_set_from", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {camera, point});
    REQUIRE(sunray::script::is_class(res));
    CHECK(sunray::script::as_class(res));
  }
  SECTION("set to")
  {
    auto point{point_meta_class->construct(3, 2, 1)};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Camera_set_to", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {camera, point});
    REQUIRE(sunray::script::is_class(res));
    CHECK(sunray::script::as_class(res));
  }
  SECTION("set up")
  {
    auto vector{vector_meta_class->construct(1, 2, 3)};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Camera_set_up", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {camera, vector});
    REQUIRE(sunray::script::is_class(res));
    CHECK(sunray::script::as_class(res));
  }
  SECTION("set fov")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Camera_set_field_of_view", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {camera, 1.67});
    REQUIRE(sunray::script::is_class(res));
    CHECK(sunray::script::as_class(res));
  }
  SECTION("set horizontal")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Camera_set_horizontal", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {camera, 200.0});
    REQUIRE(sunray::script::is_class(res));
    CHECK(sunray::script::as_class(res));
  }
  SECTION("get horizontal")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Camera_get_horizontal", 1));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {camera});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(500));
  }
  SECTION("set vertical")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Camera_set_vertical", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {camera, 500.0});
    REQUIRE(sunray::script::is_class(res));
    CHECK(sunray::script::as_class(res));
  }
  SECTION("render world")
  {
    auto world_meta_class = std::make_shared<sunray::script::WorldMetaClass>();
    auto world = world_meta_class->construct();
    auto light = std::make_shared<sunray::PointLight>(sunray::create_point(0, 5, -10.0), sunray::Color(1, 1, 1));
    auto sphere =
      std::make_shared<sunray::Sphere>(sunray::Material{sunray::Color{1, 1, 1}, 1, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f});

    world->add(light);
    world->add(sphere);
    camera->horizontal(20);
    camera->vertical(10);

    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Camera_render", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {camera, world});
    REQUIRE(sunray::script::is_class(res));
    auto canvas = std::dynamic_pointer_cast<sunray::script::Canvas>(sunray::script::as_class(res));
    REQUIRE(canvas);
    CHECK(canvas->height() == Approx(10));
    CHECK(canvas->width() == Approx(20));
  }
}

TEST_CASE("camera stream", "[camera]")
{
  auto camera_meta_class = std::make_shared<sunray::script::CameraMetaClass>();

  SECTION("to string")
  {
    auto camera{camera_meta_class->construct()};
    CHECK(camera->to_string() == "Camera h: 500 v: 250 fov: 1.0471975511965976 f: {x: 0 y: 1.5 z: 0.7 w: 1} t: {x: 0 y: 1 z: 0 "
                                 "w: 1} u: {x: 0 y: 1 z: 0 w: 0}");
  }
}
