//
//  function_registry_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 30.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/build_in_functions.h>
#include <sun_ray/script/function_registry.h>

#include <sstream>
#include <thread>

#include <catch2/catch.hpp>

using namespace std::string_literals;


namespace
{
  double my_function()
  {
    return 815.0;
  }

  std::string my_other_function(double d, const std::string& s)
  {
    CHECK(d == Approx(4711.0));
    CHECK(s == "my string");
    return "hutzli";
  }

  double my_multi_function(double d1, const std::string& s1, double d2, const std::string& s2)
  {
    CHECK(d1 == Approx(4));
    CHECK(s1 == "my string");
    CHECK(d2 == Approx(8.15));
    CHECK(s2 == "second string");

    return 47.11;
  }
}

TEST_CASE("function registry", "[function registry]")
{
  SECTION("add functions")
  {
    sunray::script::FunctionRegistry registry;
    CHECK(registry.add_function("my_function", my_function) == 0);
    CHECK(registry.add_function("my_other_function", my_other_function) == 1);
  }
  SECTION("add same function")
  {
    sunray::script::FunctionRegistry registry;
    CHECK(registry.add_function("my_function", my_function) == 0);
    CHECK(registry.add_function("my_other_function", my_other_function) == 1);
    CHECK(registry.add_function("my_function", my_function) == -1);
  }
  SECTION("get functions with index")
  {
    sunray::script::FunctionRegistry registry;
    auto idx_my_func = registry.add_function("my_function", my_function);
    auto idx_my_other_func = registry.add_function("my_other_function", my_other_function);
    auto idx_my_multi_func = registry.add_function("my_multi_function", my_multi_function);

    CHECK(registry.index_for_function(sunray::script::NameMangler::mangle("my_function", 0)) == idx_my_func);
    CHECK(registry.index_for_function(sunray::script::NameMangler::mangle("my_other_function", 2)) == idx_my_other_func);
    CHECK(registry.index_for_function(sunray::script::NameMangler::mangle("my_multi_function", 4)) == idx_my_multi_func);
    CHECK(registry.index_for_function("unknown") == -1);
  }
  SECTION("call functions")
  {
    sunray::script::FunctionRegistry registry;
    auto idx_my_func = static_cast<size_t>(registry.add_function("my_function", my_function));
    auto idx_my_other_func = static_cast<size_t>(registry.add_function("my_other_function", my_other_function));
    auto idx_my_multi_func = static_cast<size_t>(registry.add_function("my_multi_function", my_multi_function));

    auto res = registry.call_function(idx_my_func, {});
    CHECK(sunray::script::as_double(res) == Approx(815));
    res = registry.call_function(idx_my_other_func, {4711.0, "my string"s});
    res = registry.call_function(idx_my_multi_func, {4.0, "my string"s, 8.15, "second string"s});
  }
}

TEST_CASE("function registry error", "[function registry]")
{
  sunray::script::FunctionRegistry registry;
  auto idx_my_func = static_cast<size_t>(registry.add_function("my_function", my_function));
  auto idx_my_other_func = static_cast<size_t>(registry.add_function("my_other_function", my_other_function));

  SECTION("call function wrong argument number")
  {
    CHECK_THROWS_WITH(registry.call_function(idx_my_func, {4711.0}),
                      "Function 'my_function' error: Called with wrong parameter count. Should be 0, but is 1.");
  }
  SECTION("call function wrong argument type")
  {
    CHECK_THROWS_WITH(
      registry.call_function(idx_my_other_func, {"my string"s, 4711.0}),
      "Function 'my_other_function' error: Called with wrong parameter type for parameter 0. Should be double, but is string.");
  }
}


TEST_CASE("now function", "[build in functions]")
{
  std::stringstream ss;
  sunray::script::FunctionRegistry registry;
  sunray::script::BuildInFunctions(registry, ss);
  auto idx_now = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("now", 0)));

  auto idx_str = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("str", 1)));
  auto idx_fmt = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("format", 2)));
  auto idx_random = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("random", 0)));
  auto idx_seed = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("seed", 1)));

  auto idx_print = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("print", 1)));
  auto idx_print2 = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("print", 2)));
  auto idx_print3 = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("print", 3)));
  auto idx_print4 = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("print", 4)));
  auto idx_print5 = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("print", 5)));

  auto idx_println = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("println", 1)));
  auto idx_println2 = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("println", 2)));
  auto idx_println3 = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("println", 3)));
  auto idx_println4 = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("println", 4)));
  auto idx_println5 = static_cast<size_t>(registry.index_for_function(sunray::script::NameMangler::mangle("println", 5)));

  SECTION("random function")
  {
    auto res = registry.call_function(idx_random, {});
    CHECK(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(-0.0992494558));
    res = registry.call_function(idx_random, {});
    CHECK(sunray::script::as_double(res) == Approx(0.7814232008));
    res = registry.call_function(idx_random, {});
    CHECK(sunray::script::as_double(res) == Approx(-0.6738933005));
    res = registry.call_function(idx_random, {});
    CHECK(sunray::script::as_double(res) == Approx(0.5435631699));
  }
  SECTION("seed function")
  {
    auto res = registry.call_function(idx_seed, {1817177213.0});
    CHECK(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));

    res = registry.call_function(idx_random, {});
    CHECK(sunray::script::as_double(res) == Approx(0.5517534396));
    res = registry.call_function(idx_random, {});
    CHECK(sunray::script::as_double(res) == Approx(0.2428256923));
    res = registry.call_function(idx_random, {});
    CHECK(sunray::script::as_double(res) == Approx(-0.7330671681));
    res = registry.call_function(idx_random, {});
    CHECK(sunray::script::as_double(res) == Approx(-0.1704434534));
  }
  SECTION("format function")
  {
    auto res = registry.call_function(idx_fmt, {"{:03.0f}"s, 47.11});
    CHECK(sunray::script::is_string(res));
    CHECK(sunray::script::as_string(res) == "047");

    res = registry.call_function(idx_fmt, {"{}"s, true});
    CHECK(sunray::script::is_string(res));
    CHECK(sunray::script::as_string(res) == "true");

    res = registry.call_function(idx_fmt, {"{}"s, "check!"s});
    CHECK(sunray::script::is_string(res));
    CHECK(sunray::script::as_string(res) == "check!");
  }
  SECTION("str function")
  {
    auto res = registry.call_function(idx_str, {47.11});
    CHECK(sunray::script::is_string(res));
    CHECK(sunray::script::as_string(res) == "47.11");

    res = registry.call_function(idx_str, {"47.11"s});
    CHECK(sunray::script::is_string(res));
    CHECK(sunray::script::as_string(res) == "47.11");

    res = registry.call_function(idx_str, {true});
    CHECK(sunray::script::is_string(res));
    CHECK(sunray::script::as_string(res) == "true");
  }
  SECTION("now function")
  {
    auto res = registry.call_function(idx_now, {});
    CHECK(sunray::script::is_string(res));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    auto res2 = registry.call_function(idx_now, {});
    CHECK(sunray::script::is_string(res2));
    CHECK(sunray::script::as_string(res) != sunray::script::as_string(res2));
  }
  SECTION("print simple")
  {
    auto res = registry.call_function(idx_print, {"42 is the best number ever!"s});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(ss.str() == "42 is the best number ever!");
  }
  SECTION("print one arg")
  {
    auto res = registry.call_function(idx_print2, {"{} is the best number ever!"s, 42.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(ss.str() == "42.00 is the best number ever!");
  }
  SECTION("print two args")
  {
    auto res = registry.call_function(idx_print3, {"My name is {0}, {1} {0}"s, "Bond"s, "James"s});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(ss.str() == "My name is Bond, James Bond");
  }
  SECTION("print three args")
  {
    auto res = registry.call_function(idx_print4, {"Fancy numbers: {}, {}, and {}"s, 42.0, 4711.0, "0815"s});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(ss.str() == "Fancy numbers: 42.00, 4711.00, and 0815");
  }
  SECTION("print four args")
  {
    auto res = registry.call_function(idx_print5, {"Fancy numbers: {}, {}, {}, and {}"s, 42.0, 4711.0, "0815"s, "17+4"s});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(ss.str() == "Fancy numbers: 42.00, 4711.00, 0815, and 17+4");
  }
  SECTION("print too many args")
  {
    CHECK_THROWS_WITH(registry.call_function(idx_print4, {"Numbers: {}, {}, {}, {}, {}, {}, {}, {}, {}"s, 1.0, 2.0, 3.0, 4.0, 5.0,
                                                          6.0, 7.0, 8.0, 9.0}),
                      "Function 'print' error: Called with wrong parameter count. Should be 4, but is 10.");
  }
  SECTION("println simple")
  {
    auto res = registry.call_function(idx_println, {"My name is Bond, James Bond"s});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(ss.str() == "My name is Bond, James Bond\n");
  }
  SECTION("print one arg")
  {
    auto res = registry.call_function(idx_println2, {"{} is the best number ever!"s, 42.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(ss.str() == "42.00 is the best number ever!\n");
  }
  SECTION("println two args")
  {
    auto res = registry.call_function(idx_println3, {"My name is {0}, {1} {0}"s, "Bond"s, "James"s});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(ss.str() == "My name is Bond, James Bond\n");
  }
  SECTION("println three args")
  {
    auto res = registry.call_function(idx_println4, {"Fancy numbers: {}, {}, and {}"s, 42.0, 4711.0, "0815"s});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(ss.str() == "Fancy numbers: 42.00, 4711.00, and 0815\n");
  }
  SECTION("println four args")
  {
    auto res = registry.call_function(idx_println5, {"Fancy numbers: {}, {}, {}, and {}"s, 42.0, 4711.0, "0815"s, "17+4"s});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK(ss.str() == "Fancy numbers: 42.00, 4711.00, 0815, and 17+4\n");
  }
}
