//
//  helper_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 22.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/math_helper.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("approx", "[math helper]")
{
  SECTION("float equal")
  {
    sunray::Approx a{47.11f};
    CHECK(a == 47.110001f);
    CHECK_FALSE(a == 47.1102f);
  }
  SECTION("double equal")
  {
    sunray::Approx a{47.11};
    CHECK(a == 47.110001);
    CHECK_FALSE(a == 47.1102);
  }
}

TEST_CASE("math", "[math helper]")
{
  SECTION("deg to rad")
  {
    CHECK(sunray::deg_to_rad(25.0) == Approx(0.4363323));
  }
  SECTION("rad to deg")
  {
    CHECK(sunray::rad_to_deg(0.4363323) == Approx(25.0));
  }
}
