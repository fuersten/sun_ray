//
//  name_mangler_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 04.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/name_mangler.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("mangle", "[name mangler]")
{
  SECTION("simple")
  {
    CHECK(sunray::script::NameMangler::mangle("println") == "_SR7println");
  }
  SECTION("function with parameter")
  {
    CHECK(sunray::script::NameMangler::mangle("other-func", 2) == "_SR10other-func2");
    CHECK(sunray::script::NameMangler::mangle("my-func", 6) == "_SR7my-func6");
  }
}

TEST_CASE("demangle", "[name mangler]")
{
  SECTION("simple")
  {
    CHECK(sunray::script::NameMangler::demangle("_SR7println") == "println");
    CHECK(sunray::script::NameMangler::demangle("_SR10other-func") == "other-func");
  }
  SECTION("function with parameter")
  {
    CHECK(sunray::script::NameMangler::demangle("_SR10other-func2") == "other-func");
    CHECK(sunray::script::NameMangler::demangle("_SR7my-func6") == "my-func");
  }
}

TEST_CASE("mangle error", "[name mangler]")
{
  SECTION("empty name")
  {
    CHECK_THROWS_WITH(sunray::script::NameMangler::mangle(""), "cannot mangle empty name");
  }
}

TEST_CASE("demangle error", "[name mangler]")
{
  SECTION("unmangeled name")
  {
    CHECK_THROWS_WITH(sunray::script::NameMangler::demangle("println"), "'println' is not a mangeled name");
    CHECK_THROWS_WITH(sunray::script::NameMangler::demangle("_EP7println"), "'_EP7println' is not a mangeled name");
    CHECK_THROWS_WITH(sunray::script::NameMangler::demangle("_SR736"), "'_SR736' is not a mangeled name");
    CHECK_THROWS_WITH(sunray::script::NameMangler::demangle("_SR0println"), "'_SR0println' is not a mangeled name");
    CHECK_THROWS_WITH(sunray::script::NameMangler::demangle("_SRprintln"), "'_SRprintln' is not a mangeled name");
  }
}
