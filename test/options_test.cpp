//
//  options_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 11.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sstream>

#include <catch2/catch.hpp>

#include <src/sun_ray/options.h>


TEST_CASE("options", "[options]")
{
  std::stringstream stream;

  SECTION("process no options")
  {
    std::vector<std::string> args{"app"};

    auto opts = sunray::Options::handle_options(stream, args);
    CHECK(opts.first);
    CHECK_FALSE(opts.second.dump_);
    CHECK_FALSE(opts.second.format_);
    CHECK(opts.second.files_.empty());
    CHECK(stream.str().empty());
  }
  SECTION("process files options")
  {
    std::vector<std::string> args{"app", "script1.wsl", "script2.wsl", "script3.wsl"};

    auto opts = sunray::Options::handle_options(stream, args);
    CHECK(opts.first);
    CHECK_FALSE(opts.second.dump_);
    CHECK_FALSE(opts.second.format_);
    CHECK(opts.second.files_.size() == 3);
    CHECK(stream.str().empty());
  }
  SECTION("process short dump option")
  {
    std::vector<std::string> args{"app", "-d"};

    auto opts = sunray::Options::handle_options(stream, args);
    CHECK(opts.first);
    CHECK(opts.second.dump_);
    CHECK_FALSE(opts.second.format_);
    CHECK(opts.second.files_.empty());
    CHECK(stream.str().empty());
  }
  SECTION("process long dump option")
  {
    std::vector<std::string> args{"app", "--dump"};

    auto opts = sunray::Options::handle_options(stream, args);
    CHECK(opts.first);
    CHECK(opts.second.dump_);
    CHECK_FALSE(opts.second.format_);
    CHECK(opts.second.files_.empty());
    CHECK(stream.str().empty());
  }
  SECTION("process short format option")
  {
    std::vector<std::string> args{"app", "-f"};

    auto opts = sunray::Options::handle_options(stream, args);
    CHECK(opts.first);
    CHECK_FALSE(opts.second.dump_);
    CHECK(opts.second.format_);
    CHECK(opts.second.files_.empty());
    CHECK(stream.str().empty());
  }
  SECTION("process long dump option")
  {
    std::vector<std::string> args{"app", "--format"};

    auto opts = sunray::Options::handle_options(stream, args);
    CHECK(opts.first);
    CHECK_FALSE(opts.second.dump_);
    CHECK(opts.second.format_);
    CHECK(opts.second.files_.empty());
    CHECK(stream.str().empty());
  }
  SECTION("process all options")
  {
    std::vector<std::string> args{"app", "-d", "-f", "script1.wsl", "script2.wsl", "script3.wsl"};

    auto opts = sunray::Options::handle_options(stream, args);
    CHECK(opts.first);
    CHECK(opts.second.dump_);
    CHECK(opts.second.format_);
    CHECK(opts.second.files_.size() == 3);
    CHECK(stream.str().empty());
  }
}

TEST_CASE("options usage", "[options]")
{
  std::stringstream stream;

  SECTION("usage")
  {
    sunray::Options::print_usage(stream);
    CHECK_FALSE(stream.str().empty());
    auto expected = R"(Usage: sun_ray [ --help ] | [ [-df] <FILE> [<FILE>]... ]
help:
  --help                              display this help and exit

processing options:
  -d, --dump                          dump instructions
  -f, --format                        format the program
  <FILE>                              script to execute

)";

    CHECK(stream.str() == expected);
  }
}

TEST_CASE("error options", "[options]")
{
  std::stringstream stream;

  SECTION("process no options")
  {
    std::vector<std::string> args{};

    auto opts = sunray::Options::handle_options(stream, args);
    CHECK_FALSE(opts.first);
    CHECK_FALSE(opts.second.dump_);
    CHECK_FALSE(opts.second.format_);
    CHECK(opts.second.files_.empty());
    CHECK_FALSE(stream.str().empty());
  }
  SECTION("process wrong options")
  {
    std::vector<std::string> args{"app", "-j"};

    auto opts = sunray::Options::handle_options(stream, args);
    CHECK_FALSE(opts.first);
    CHECK_FALSE(opts.second.dump_);
    CHECK_FALSE(opts.second.format_);
    CHECK(opts.second.files_.empty());
    CHECK_FALSE(stream.str().empty());
  }
  SECTION("process after untagged options")
  {
    std::vector<std::string> args{"app", "script.wsl", "-f", "-d"};

    auto opts = sunray::Options::handle_options(stream, args);
    CHECK_FALSE(opts.first);
    CHECK_FALSE(opts.second.dump_);
    CHECK_FALSE(opts.second.format_);
    CHECK(opts.second.files_.empty());
    CHECK_FALSE(stream.str().empty());
  }
}
