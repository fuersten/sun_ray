//
//  point_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 15.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/objects/point.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("point construction", "[point]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::PointMetaClass>());

  SECTION("construct point")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Point_constructor", 1));
    REQUIRE(idx != -1);
    auto point = function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 1.0});
    REQUIRE(sunray::script::is_class(point));
  }
  SECTION("construct point error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Point_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("point methods", "[point]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto point_meta_class = std::make_shared<sunray::script::PointMetaClass>();
  registry.add_meta_class(point_meta_class);
  auto vector_meta_class = std::make_shared<sunray::script::VectorMetaClass>();
  auto point{point_meta_class->construct(-2, 3, 1)};

  SECTION("add vector")
  {
    auto vector{vector_meta_class->construct(3, -2, 5)};

    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Point_add", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {point, vector});
    REQUIRE(sunray::script::is_class(res));
    auto p = std::dynamic_pointer_cast<sunray::script::Point>(sunray::script::as_class(res));
    REQUIRE(p);
    CHECK(p->x() == Approx(1));
    CHECK(p->y() == Approx(1));
    CHECK(p->z() == Approx(6));
  }
  SECTION("add error")
  {
    auto point_to_add{point_meta_class->construct(-2, 3, 1)};

    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Point_add", 2));
    CHECK_THROWS_WITH(function_registry.call_function(static_cast<size_t>(idx), {point, point_to_add}),
                      "Function 'Point_add' error: Point add wrong parameter for vector. Should be Vector but is Point.");
  }
  SECTION("getter")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Point_get_x", 1));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {point});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(-2));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Point_get_y", 1));
    res = function_registry.call_function(static_cast<size_t>(idx), {point});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(3));

    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Point_get_z", 1));
    res = function_registry.call_function(static_cast<size_t>(idx), {point});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(1));
  }
}

TEST_CASE("point stream", "[point]")
{
  auto point_meta_class = std::make_shared<sunray::script::PointMetaClass>();

  SECTION("to string")
  {
    auto point{point_meta_class->construct(0.0, -0.1, 1.0)};
    CHECK(point->to_string() == "Point x: 0 y: -0.1 z: 1");
  }
}
