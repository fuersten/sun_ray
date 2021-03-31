//
//  disk_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 19.04.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/objects/disk.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("disk construction", "[disk]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  registry.add_meta_class(std::make_shared<sunray::script::DiskMetaClass>());
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("construct disk")
  {
    auto material{material_meta_class->construct()};
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Disk_constructor", 1));
    REQUIRE(idx != -1);
    auto disk = function_registry.call_function(static_cast<size_t>(idx), {material});
    REQUIRE(sunray::script::is_class(disk));
  }
  SECTION("construct disk error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Disk_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("disk methods", "[disk]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto disk_meta_class = std::make_shared<sunray::script::DiskMetaClass>();
  registry.add_meta_class(disk_meta_class);
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();
  auto material{material_meta_class->construct()};
  auto disk{disk_meta_class->construct(material)};

  SECTION("set inner_radius")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Disk_set_inner_radius", 2));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {disk, 0.2});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(disk->inner_radius() == Approx(0.2));
  }
  SECTION("transform")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Disk_scale", 4));
    auto res = function_registry.call_function(static_cast<size_t>(idx), {disk, 5.0, 5.0, 5.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Disk_shear", 7));
    res = function_registry.call_function(static_cast<size_t>(idx), {disk, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Disk_translate", 4));
    res = function_registry.call_function(static_cast<size_t>(idx), {disk, -1.0, 0.0, 1.0});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Disk_rotate_x", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {disk, sunray::PI / 4});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Disk_rotate_y", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {disk, sunray::PI / 2});
    idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Disk_rotate_z", 2));
    res = function_registry.call_function(static_cast<size_t>(idx), {disk, sunray::PI});
    REQUIRE(sunray::script::is_class(res));
    auto p = std::dynamic_pointer_cast<sunray::script::Disk>(sunray::script::as_class(res));
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
    CHECK(std::dynamic_pointer_cast<const sunray::Disk>(o));
  }
}

TEST_CASE("disk stream", "[disk]")
{
  auto disk_meta_class = std::make_shared<sunray::script::DiskMetaClass>();
  auto material_meta_class = std::make_shared<sunray::script::MaterialMetaClass>();

  SECTION("to string")
  {
    auto material{material_meta_class->construct()};

    auto disk{disk_meta_class->construct(material)};
    CHECK(disk->to_string() == "Disk");
  }
}
