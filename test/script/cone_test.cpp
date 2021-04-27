//
//  cone_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 27.04.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/objects/cone.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("cone construction", "[cone]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::ConeMetaClass>());
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("construct cone")
  {
    auto material{material_meta_class->construct()};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_constructor", 1));
    REQUIRE(idx != -1);
    auto cone = function_registry.call_function(static_cast<size_t>(idx), {material});
    REQUIRE(sunray::script::is_class(cone));
  }
  SECTION("construct cone error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("cone methods", "[cone]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto cone_meta_class = std::make_shared<sunray::script::ConeMetaClass>();
  registry.add_meta_class(cone_meta_class);
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  auto material{material_meta_class->construct()};
  auto cone{cone_meta_class->construct(material)};

  SECTION("set maximum")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_set_maximum", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {cone, 2.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(cone->maximum() == Approx(2));
  }
  SECTION("set minimum")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_set_minimum", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {cone, 1.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(cone->minimum() == Approx(1));
  }
  SECTION("set closed")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_set_closed", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {cone, true});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(cone->closed());
  }
  SECTION("transform")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_scale", 4));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {cone, 5.0, 5.0, 5.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_shear", 7));
    res = function_registry.call_function(static_cast<size_t>(idx), {cone, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_translate", 4));
    res = function_registry.call_function(static_cast<size_t>(idx), {cone, -1.0, 0.0, 1.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_rotate_x", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {cone, sunray::PI / 4});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_rotate_y", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {cone, sunray::PI / 2});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cone_rotate_z", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {cone, sunray::PI});
    REQUIRE(sunray::script::is_class(res));
    auto p = std::dynamic_pointer_cast<sunray::script::Cone>(sunray::script::as_class(res));
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
    CHECK(std::dynamic_pointer_cast<const sunray::Cone>(o));
  }
}

TEST_CASE("cone stream", "[cone]")
{
  auto cone_meta_class = std::make_shared<sunray::script::ConeMetaClass>();
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("to string")
  {
    auto material{material_meta_class->construct()};

    auto cone{cone_meta_class->construct(material)};
    CHECK(cone->to_string() == "Cone max: inf min: -inf closed: false");

    cone->maximum(2.0);
    cone->minimum(1.0);
    cone->closed(true);
    CHECK(cone->to_string() == "Cone max: 2 min: 1 closed: true");
  }
}
