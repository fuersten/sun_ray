//
//  location_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 28.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/location.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("create location", "[location]")
{
  SECTION("create default location")
  {
    sunray::script::Location location;
    CHECK(location.line() == 1);
    CHECK(location.column() == 1);
  }
  SECTION("create location")
  {
    sunray::script::Location location{47, 11};
    CHECK(location.line() == 47);
    CHECK(location.column() == 11);
  }
}

TEST_CASE("locationp operator", "[location]")
{
  SECTION("equal location")
  {
    sunray::script::Location location{47, 11};
    sunray::script::Location location1{47, 11};

    CHECK(location == location1);
    CHECK_FALSE(location == sunray::script::Location{11, 11});
  }
}

TEST_CASE("stream location", "[location]")
{
  SECTION("stream location")
  {
    sunray::script::Location location{47, 11};
    std::stringstream ss;
    ss << location;
    CHECK("[47:11]" == ss.str());
  }
}
