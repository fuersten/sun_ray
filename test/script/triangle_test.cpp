//
//  triangle_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 31.03.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/objects/triangle.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("triangle construction", "[triangle]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::TriangleMetaClass>());
  auto point_meta_class{std::make_shared<sunray::script::PointMetaClass>()};
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("construct triangle")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Triangle_constructor", 1));
    REQUIRE(idx != -1);
    auto material{material_meta_class->construct()};
    auto p1{point_meta_class->construct(-2, 3, 1)};
    auto p2{point_meta_class->construct(2, -3, 1)};
    auto p3{point_meta_class->construct(2, 3, -1)};
    auto triangle = function_registry.call_function(static_cast<size_t>(idx), {material, p1, p2, p3});
    REQUIRE(sunray::script::is_class(triangle));
  }
  SECTION("construct triangle error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Triangle_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("triangle methods", "[triangle]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto triangle_meta_class = std::make_shared<sunray::script::TriangleMetaClass>();
  registry.add_meta_class(triangle_meta_class);
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  auto material{material_meta_class->construct()};
  auto point_meta_class{std::make_shared<sunray::script::PointMetaClass>()};
  auto p1{point_meta_class->construct(-2, 3, 1)};
  auto p2{point_meta_class->construct(2, -3, 1)};
  auto p3{point_meta_class->construct(2, 3, -1)};
  auto triangle{triangle_meta_class->construct(material, p1, p2, p3)};

  SECTION("transform")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Triangle_scale", 4));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {triangle, 5.0, 5.0, 5.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Triangle_shear", 7));
    res = function_registry.call_function(static_cast<size_t>(idx), {triangle, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Triangle_translate", 4));
    res = function_registry.call_function(static_cast<size_t>(idx), {triangle, -1.0, 0.0, 1.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Triangle_rotate_x", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {triangle, sunray::PI / 4});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Triangle_rotate_y", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {triangle, sunray::PI / 2});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Triangle_rotate_z", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {triangle, sunray::PI});
    REQUIRE(sunray::script::is_class(res));
    auto p = std::dynamic_pointer_cast<sunray::script::Triangle>(sunray::script::as_class(res));
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
    CHECK(std::dynamic_pointer_cast<const sunray::Triangle>(o));
  }
}

TEST_CASE("triangle stream", "[triangle]")
{
  auto triangle_meta_class = std::make_shared<sunray::script::TriangleMetaClass>();
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  auto point_meta_class{std::make_shared<sunray::script::PointMetaClass>()};
  auto p1{point_meta_class->construct(-2, 3, 1)};
  auto p2{point_meta_class->construct(2, -3, 1)};
  auto p3{point_meta_class->construct(2, 3, -1)};

  SECTION("to string")
  {
    auto material{material_meta_class->construct()};

    auto triangle{triangle_meta_class->construct(material, p1, p2, p3)};
    CHECK(triangle->to_string() == "Triangle");
  }
}
