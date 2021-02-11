//
//  light_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 12.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/light.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("create lights", "[lights]")
{
  SECTION("create light")
  {
    sunray::PointLight light{sunray::create_point(0, 0, 0), sunray::Color(1, 1, 1)};
    CHECK(light.position() == sunray::create_point(0, 0, 0));
    CHECK(light.intensity() == sunray::Color(1, 1, 1));
  }
}

TEST_CASE("stream light", "[lights]")
{
  SECTION("stream")
  {
    sunray::PointLight light{sunray::create_point(0, 5, -10.0), sunray::Color(1, 1, 1)};
    std::stringstream ss;
    ss << light;
    CHECK("position: x: 0 y: 5 z: -10 w: 1 intensity: red: 1 green: 1 blue: 1" == ss.str());
  }
}
