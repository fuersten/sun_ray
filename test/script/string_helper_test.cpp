//
//  strng_helper_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 22.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/string_helper.h>

#include <catch2/catch.hpp>


TEST_CASE("trimming", "[string helper]")
{
  SECTION("trim left")
  {
    std::string s("     left whitespace ");
    CHECK("left whitespace " == sunray::script::trim_left(s));
    s = "no left whitespace";
    CHECK("no left whitespace" == sunray::script::trim_left(s));
  }
  SECTION("trim right")
  {
    std::string s = " right whitespace    ";
    CHECK(" right whitespace" == sunray::script::trim_right(s));
    s = "no right whitespace";
    CHECK("no right whitespace" == sunray::script::trim_right(s));
  }
  SECTION("trim")
  {
    std::string s = " trim the whitespaces    ";
    CHECK("trim the whitespaces" == sunray::script::trim(s));
    s = "no whitespaces to trim";
    CHECK("no whitespaces to trim" == sunray::script::trim_right(s));
  }
  SECTION("trim only left")
  {
    std::string s = " trim the whitespaces";
    CHECK("trim the whitespaces" == sunray::script::trim(s));
  }
  SECTION("trim only right")
  {
    std::string s = "trim the whitespaces    ";
    CHECK("trim the whitespaces" == sunray::script::trim(s));
  }
}

TEST_CASE("join", "[string helper]")
{
  SECTION("join empty")
  {
    std::vector<int> v{};
    CHECK("" == sunray::script::join(v, ", "));
  }
  SECTION("join single element")
  {
    std::vector<int> v{5};
    CHECK("5" == sunray::script::join(v, ", "));
  }
  SECTION("join ints")
  {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
    CHECK("1, 2, 3, 4, 5, 6, 7, 8" == sunray::script::join(v, ", "));
  }
  SECTION("join string")
  {
    std::vector<std::string> v{"hutzli", "putzli", "schlitzli"};
    CHECK("hutzli/putzli/schlitzli" == sunray::script::join(v, "/"));
  }
}
