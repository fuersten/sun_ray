//
//  function_object_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 07.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/format_helper.h>
#include <sun_ray/script/function_object.h>

#include <algorithm>
#include <cmath>
#include <sstream>

#include <catch2/catch.hpp>

#include <date/date.h>


static std::string do_double(double f)
{
  return fmt::format("done it with {}", f);
}

static std::string now()
{
  auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
  return date::format("%FT%T", now);
}

static sunray::script::Variant call_function(sunray::script::Functions& funcs, const std::string& name,
                                             const std::vector<sunray::script::Variant>& parameter)
{
  auto it = std::find_if(funcs.begin(), funcs.end(), [&name](const auto& func) {
    return func.first == name;
  });
  if (it == funcs.end()) {
    throw std::runtime_error{fmt::format("function {} not found", name)};
  }

  return it->second->call(parameter);
}


TEST_CASE("generic function call", "[function registry]")
{
  sunray::script::Functions funcs;
  sunray::script::registerFunction(funcs, "do_double", do_double);
  sunray::script::registerFunction(funcs, "now", now);
  sunray::script::registerFunction(funcs, "pow", (double (*)(double, double))(pow));

  SECTION("function call without parameter")
  {
    std::vector<sunray::script::Variant> parameters;

    auto res = call_function(funcs, "now", parameters);
    REQUIRE(sunray::script::is_string(res));
  }
  SECTION("function call with one parameter")
  {
    std::vector<sunray::script::Variant> parameters = {47.11};

    auto res = call_function(funcs, "do_double", parameters);
    REQUIRE(sunray::script::is_string(res));
    CHECK(sunray::script::as_string(res) == "done it with 47.11");
  }
  SECTION("function call with two parameters")
  {
    std::vector<sunray::script::Variant> parameters = {7.0, 3.0};

    auto res = call_function(funcs, "pow", parameters);
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(343));
  }
}

static double call_zero()
{
  return 0;
}

static double call_one(double one)
{
  CHECK(one == Approx(1));
  return 1;
}

static double call_two(double one, double two)
{
  CHECK(one == Approx(1));
  CHECK(two == Approx(2));
  return 2;
}

static double call_three(double one, double two, double three)
{
  CHECK(one == Approx(1));
  CHECK(two == Approx(2));
  CHECK(three == Approx(3));
  return 3;
}

static double call_four(double one, double two, double three, double four)
{
  CHECK(one == Approx(1));
  CHECK(two == Approx(2));
  CHECK(three == Approx(3));
  CHECK(four == Approx(4));
  return 4;
}

static double call_five(double one, double two, double three, double four, double five)
{
  CHECK(one == Approx(1));
  CHECK(two == Approx(2));
  CHECK(three == Approx(3));
  CHECK(four == Approx(4));
  CHECK(five == Approx(5));
  return 5;
}

static double call_six(double one, double two, double three, double four, double five, double six)
{
  CHECK(one == Approx(1));
  CHECK(two == Approx(2));
  CHECK(three == Approx(3));
  CHECK(four == Approx(4));
  CHECK(five == Approx(5));
  CHECK(six == Approx(6));
  return 6;
}

static double call_seven(double one, double two, double three, double four, double five, double six, double seven)
{
  CHECK(one == Approx(1));
  CHECK(two == Approx(2));
  CHECK(three == Approx(3));
  CHECK(four == Approx(4));
  CHECK(five == Approx(5));
  CHECK(six == Approx(6));
  CHECK(seven == Approx(7));
  return 7;
}


TEST_CASE("generic function checks", "[function registry]")
{
  sunray::script::Functions funcs;
  sunray::script::registerFunction(funcs, "call_zero", call_zero);
  sunray::script::registerFunction(funcs, "call_one", call_one);
  sunray::script::registerFunction(funcs, "call_two", call_two);
  sunray::script::registerFunction(funcs, "call_three", call_three);
  sunray::script::registerFunction(funcs, "call_four", call_four);
  sunray::script::registerFunction(funcs, "call_five", call_five);
  sunray::script::registerFunction(funcs, "call_six", call_six);
  sunray::script::registerFunction(funcs, "call_seven", call_seven);

  SECTION("function call zero")
  {
    auto res = call_function(funcs, "call_zero", {});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(0));
    CHECK_THROWS_WITH(call_function(funcs, "call_zero", {1.0}), "Called with wrong parameter count. Should be 0, but is 1.");
  }
  SECTION("function call one")
  {
    auto res = call_function(funcs, "call_one", {1.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(1));
    CHECK_THROWS_WITH(call_function(funcs, "call_one", {1.0, 2.0}), "Called with wrong parameter count. Should be 1, but is 2.");
  }
  SECTION("function call two")
  {
    auto res = call_function(funcs, "call_two", {1.0, 2.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(2));
    CHECK_THROWS_WITH(call_function(funcs, "call_two", {1.0}), "Called with wrong parameter count. Should be 2, but is 1.");
  }
  SECTION("function call three")
  {
    auto res = call_function(funcs, "call_three", {1.0, 2.0, 3.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(3));
    CHECK_THROWS_WITH(call_function(funcs, "call_three", {1.0, 2.0}),
                      "Called with wrong parameter count. Should be 3, but is 2.");
  }
  SECTION("function call four")
  {
    auto res = call_function(funcs, "call_four", {1.0, 2.0, 3.0, 4.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(4));
    CHECK_THROWS_WITH(call_function(funcs, "call_four", {1.0, 2.0}), "Called with wrong parameter count. Should be 4, but is 2.");
  }
  SECTION("function call five")
  {
    auto res = call_function(funcs, "call_five", {1.0, 2.0, 3.0, 4.0, 5.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(5));
    CHECK_THROWS_WITH(call_function(funcs, "call_five", {1.0, 2.0}), "Called with wrong parameter count. Should be 5, but is 2.");
  }
  SECTION("function call six")
  {
    auto res = call_function(funcs, "call_six", {1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(6));
    CHECK_THROWS_WITH(call_function(funcs, "call_six", {1.0, 2.0}), "Called with wrong parameter count. Should be 6, but is 2.");
  }
  SECTION("function call seven")
  {
    auto res = call_function(funcs, "call_seven", {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(7));
    CHECK_THROWS_WITH(call_function(funcs, "call_seven", {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}),
                      "Called with wrong parameter count. Should be 7, but is 8.");
  }
}

TEST_CASE("generic function call lambda", "[function registry]")
{
  sunray::script::Functions funcs;
  sunray::script::registerVariadicFunction(funcs, "with_lambda", [](const std::vector<sunray::script::Variant>& params) {
    auto sum = 0.0;
    for (const auto& param : params) {
      sum += sunray::script::as_double(param);
    }
    return sum;
  });

  SECTION("call with parameter")
  {
    auto res = call_function(funcs, "with_lambda", {1.0, 2.0, 3.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(6));
    res = call_function(funcs, "with_lambda", {1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) == Approx(21));
  }
}
