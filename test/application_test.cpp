//
//  application_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 03.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <src/sun_ray/application.h>
#include <sstream>

#include "temporary_directory.h"

#include <catch2/catch.hpp>


namespace
{
  void create_input_file(TemporaryDirectoryGuard& guard, const std::string& input, const std::string& name)
  {
    auto sample_file = guard.temporary_directory_path() / name;
    std::ofstream file{sample_file.string()};
    file << input;
    file.close();
    std::filesystem::current_path(guard.temporary_directory_path());
  }
}

TEST_CASE("application", "[application]")
{
  const std::string input = R"(
      x = 42
      y = 2
      z = ((x + y) * 10) / 2
      print('z = {}', z)
  )";

  TemporaryDirectoryGuard guard;
  create_input_file(guard, input, "sample.wsl");

  SECTION("process simple script")
  {
    std::stringstream output;
    std::stringstream error;

    std::vector<std::string> args{"sun_ray", "sample.wsl"};

    sunray::Application app(output, error, args);
    CHECK(app.run() == 0);
    CHECK(output.str() == "z = 220.00");

    auto expected = "SunRay ray tracer 0.14.0\n(C)2021 Lars-Christian "
                    "Fuerstenberg\n\n================================================================================"
                    "\nsample.wsl:\n================================================================================\n\n";
    CHECK(error.str() == expected);
  }
  SECTION("process simple script with options")
  {
    std::stringstream output;
    std::stringstream error;

    std::vector<std::string> args{"sun_ray", "-f", "-d", "sample.wsl"};

    sunray::Application app(output, error, args);
    CHECK(app.run() == 0);
    CHECK(output.str() == "z = 220.00");
    CHECK_FALSE(error.str().empty());
  }
  SECTION("process help option")
  {
    std::stringstream output;
    std::stringstream error;

    std::vector<std::string> args{"sun_ray", "--help"};

    sunray::Application app(output, error, args);
    CHECK(app.run() == -1);
    CHECK(output.str().empty());
    CHECK_FALSE(error.str().empty());
  }
}

TEST_CASE("application error", "[application]")
{
  SECTION("cmd error")
  {
    std::stringstream output;
    std::stringstream error;

    std::vector<std::string> args{"sun_ray", "--unknown", "sample.wsl"};

    sunray::Application app(output, error, args);
    CHECK(app.run() == -1);
  }
  SECTION("no source")
  {
    std::stringstream output;
    std::stringstream error;

    std::vector<std::string> args{"sun_ray", "unknown.wsl"};

    sunray::Application app(output, error, args);
    CHECK(app.run() == -1);
  }
}

TEST_CASE("application process error", "[application]")
{
  SECTION("parse error")
  {
    const std::string input = R"(
        x -> check
    )";

    TemporaryDirectoryGuard guard;
    create_input_file(guard, input, "sample.wsl");

    std::stringstream output;
    std::stringstream error;

    std::vector<std::string> args{"sun_ray", "sample.wsl"};

    sunray::Application app(output, error, args);
    CHECK(app.run() == -1);
  }
  SECTION("runtime error")
  {
    const std::string input = R"(
      plane = Plane(Material())
      world = World()
      world.doit(plane)
    )";

    TemporaryDirectoryGuard guard;
    create_input_file(guard, input, "sample.wsl");

    std::stringstream output;
    std::stringstream error;

    std::vector<std::string> args{"sun_ray", "sample.wsl"};

    sunray::Application app(output, error, args);
    CHECK(app.run() == -1);
  }
}
