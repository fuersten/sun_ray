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


TEST_CASE("application", "[application]")
{
  const std::string input = R"(
      x = 42
      y = 2
      z = ((x + y) * 10) / 2
      print('z = {}', z)
  )";

  TemporaryDirectoryGuard guard;
  auto sample_file = guard.temporary_directory_path() / "sample.wsl";
  std::ofstream file{sample_file.string()};
  file << input;
  file.close();
  std::filesystem::current_path(guard.temporary_directory_path());

  SECTION("process simple script")
  {
    std::stringstream output;
    std::stringstream error;

    std::vector<std::string> args{"sun_ray", "sample.wsl"};

    sunray::Application app(output, error, args);
    CHECK(app.run() == 0);
    CHECK(output.str() == "z = 220.00");

    auto expected = "SunRay ray tracer 0.12.0\n\u00A92021 Lars-Christian "
                    "F\u00FCrstenberg\n\n================================================================================"
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
  const std::string input = R"(
      x -> check
  )";

  TemporaryDirectoryGuard guard;
  auto sample_file = guard.temporary_directory_path() / "sample.wsl";
  std::ofstream file{sample_file.string()};
  file << input;
  file.close();
  std::filesystem::current_path(guard.temporary_directory_path());

  SECTION("parse error")
  {
    std::stringstream output;
    std::stringstream error;

    std::vector<std::string> args{"sun_ray", "sample.wsl"};

    sunray::Application app(output, error, args);
    CHECK(app.run() == -1);
  }
}
