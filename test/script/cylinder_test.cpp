//
//  Cylinder_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 25.04.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/objects/cylinder.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("cylinder construction", "[cylinder]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::CylinderMetaClass>());
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("construct cylinder")
  {
    auto material{material_meta_class->construct()};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_constructor", 1));
    REQUIRE(idx != -1);
    auto cylinder = function_registry.call_function(static_cast<size_t>(idx), {material});
    REQUIRE(sunray::script::is_class(cylinder));
  }
  SECTION("construct cylinder error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("cylinder methods", "[cylinder]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto Cylinder_meta_class = std::make_shared<sunray::script::CylinderMetaClass>();
  registry.add_meta_class(Cylinder_meta_class);
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  auto material{material_meta_class->construct()};
  auto cylinder{Cylinder_meta_class->construct(material)};

  SECTION("set maximum")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_set_maximum", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {cylinder, 2.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(cylinder->maximum() == Approx(2));
  }
  SECTION("set minimum")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_set_minimum", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {cylinder, 1.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(cylinder->minimum() == Approx(1));
  }
  SECTION("set closed")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_set_closed", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {cylinder, true});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(cylinder->closed());
  }
  SECTION("transform")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_scale", 4));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {cylinder, 5.0, 5.0, 5.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_shear", 7));
    res = function_registry.call_function(static_cast<size_t>(idx), {cylinder, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_translate", 4));
    res = function_registry.call_function(static_cast<size_t>(idx), {cylinder, -1.0, 0.0, 1.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_rotate_x", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {cylinder, sunray::PI / 4});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_rotate_y", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {cylinder, sunray::PI / 2});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Cylinder_rotate_z", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {cylinder, sunray::PI});
    REQUIRE(sunray::script::is_class(res));
    auto p = std::dynamic_pointer_cast<sunray::script::Cylinder>(sunray::script::as_class(res));
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
    CHECK(std::dynamic_pointer_cast<const sunray::Cylinder>(o));
  }
}

TEST_CASE("cylinder stream", "[cylinder]")
{
  auto Cylinder_meta_class = std::make_shared<sunray::script::CylinderMetaClass>();
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("to string")
  {
    auto material{material_meta_class->construct()};

    auto cylinder{Cylinder_meta_class->construct(material)};
    CHECK(cylinder->to_string() == "Cylinder max: inf min: -inf closed: false");

    cylinder->maximum(2.0);
    cylinder->minimum(1.0);
    cylinder->closed(true);
    CHECK(cylinder->to_string() == "Cylinder max: 2 min: 1 closed: true");
  }
}
