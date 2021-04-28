//
//  vector_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 14.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/shapes/point.h>
#include <sun_ray/script/shapes/vector.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("vector construction", "[vector]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::VectorMetaClass>());

  SECTION("construct vector")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Vector_constructor", 1));
    REQUIRE(idx != -1);
    auto vector = function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 1.0});
    REQUIRE(sunray::script::is_class(vector));
  }
  SECTION("construct vector error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Vector_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("vector methods", "[vector]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto vector_meta_class = std::make_shared<sunray::script::VectorMetaClass>();
  registry.add_meta_class(vector_meta_class);

  SECTION("normalize")
  {
    auto vector{vector_meta_class->construct(4.0, 0.0, 0.0)};

    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Vector_normalize", 1));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {vector});
    REQUIRE(sunray::script::is_class(res));
    auto v = std::dynamic_pointer_cast<sunray::script::Vector>(sunray::script::as_class(res));
    REQUIRE(v);
    CHECK(v->x() == Approx(1));
    CHECK(v->y() == Approx(0));
    CHECK(v->z() == Approx(0));
  }
  SECTION("mulitply scalar")
  {
    auto vector{vector_meta_class->construct(1.0, -2.0, 3.0)};

    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Vector_multiply", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {vector, 3.5});
    REQUIRE(sunray::script::is_class(res));
    auto v = std::dynamic_pointer_cast<sunray::script::Vector>(sunray::script::as_class(res));
    REQUIRE(v);
    CHECK(v->x() == Approx(3.5));
    CHECK(v->y() == Approx(-7));
    CHECK(v->z() == Approx(10.5));
  }
  SECTION("add vector")
  {
    auto vector{vector_meta_class->construct(-2, 3, 1)};
    auto vector2{vector_meta_class->construct(3, -2, 5)};

    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Vector_add", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {vector, vector2});
    REQUIRE(sunray::script::is_class(res));
    auto v = std::dynamic_pointer_cast<sunray::script::Vector>(sunray::script::as_class(res));
    REQUIRE(v);
    CHECK(v->x() == Approx(1));
    CHECK(v->y() == Approx(1));
    CHECK(v->z() == Approx(6));
  }
  SECTION("add error")
  {
    auto point_meta_class = std::make_shared<sunray::script::PointMetaClass>();
    auto vector{vector_meta_class->construct(-2, 3, 1)};
    auto point{point_meta_class->construct(3, -2, 5)};

    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Vector_add", 2));
    CHECK_THROWS_WITH(function_registry.call_function(static_cast<size_t>(idx), {vector, point}),
                      "Function 'Vector_add' error: Vector add wrong parameter for vector. Should be Vector but is Point.");
  }
}

TEST_CASE("vector stream", "[vector]")
{
  auto vector_meta_class = std::make_shared<sunray::script::VectorMetaClass>();

  SECTION("to string")
  {
    auto vector{vector_meta_class->construct(0.0, -0.1, 1.0)};
    CHECK(vector->to_string() == "Vector x: 0 y: -0.1 z: 1");
  }
}
