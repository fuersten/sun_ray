//
//  engine_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 11.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/engine.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("process script", "[engine]")
{
  const std::string input = R"(
      x = 42
      y = 2
      z = ((x + y) * 10) / 2
      print('z = {}', z)
  )";
  std::istringstream is{input};

  SECTION("process simple script")
  {
    std::stringstream output;
    std::stringstream diagnostic_output;
    sunray::script::Engine engine{output, diagnostic_output, false, false};
    CHECK(engine.process(is));
    CHECK(output.str() == "z = 220.00");
    CHECK(diagnostic_output.str().empty());
  }
  SECTION("process simple script with diagnostic output")
  {
    std::stringstream output;
    std::stringstream diagnostic_output;
    sunray::script::Engine engine{output, diagnostic_output, true, true};
    CHECK(engine.process(is));
    CHECK(output.str() == "z = 220.00");
    CHECK_FALSE(diagnostic_output.str().empty());
  }
}

TEST_CASE("process script error", "[engine]")
{
  const std::string input = R"(
      x = 42
      y = 2
      z = ((x + y) * 10) / 2
      print('z = {}', z
  )";
  std::istringstream is{input};

  SECTION("process simple script with missing rparen")
  {
    std::stringstream output;
    std::stringstream diagnostic_output;
    sunray::script::Engine engine{output, diagnostic_output, false, false};
    CHECK_FALSE(engine.process(is));
    CHECK(output.str().empty());
    CHECK_FALSE(diagnostic_output.str().empty());
  }
}
