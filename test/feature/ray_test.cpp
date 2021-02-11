//
//  ray_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 04.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/ray.h>
#include <sun_ray/feature/transformation.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("create ray", "[ray]")
{
  SECTION("create ray")
  {
    auto origin = sunray::create_point(1, 2, 3);
    auto direction = sunray::create_vector(4, 5, 6);
    auto ray = sunray::Ray{origin, direction};
    CHECK(ray.origin() == sunray::create_point(1, 2, 3));
    CHECK(ray.direction() == sunray::create_vector(4, 5, 6));
  }
}

TEST_CASE("stream ray", "[ray]")
{
  SECTION("stream ray")
  {
    auto origin = sunray::create_point(1, 2, 3);
    auto direction = sunray::create_vector(4, 5, 6);
    auto ray = sunray::Ray{origin, direction};

    std::stringstream ss;
    ss << ray;
    CHECK("origin: x: 1 y: 2 z: 3 w: 1 direction: x: 4 y: 5 z: 6 w: 0" == ss.str());
  }
}

TEST_CASE("ray distance in time", "[ray]")
{
  SECTION("move ray")
  {
    auto ray = sunray::Ray{sunray::create_point(2, 3, 4), sunray::create_vector(1, 0, 0)};
    CHECK(ray.position(0) == sunray::create_point(2, 3, 4));
    CHECK(ray.position(1) == sunray::create_point(3, 3, 4));
    CHECK(ray.position(-1) == sunray::create_point(1, 3, 4));
    CHECK(ray.position(2.5) == sunray::create_point(4.5, 3, 4));
  }
}

TEST_CASE("transform ray", "[ray]")
{
  SECTION("translate ray")
  {
    auto ray = sunray::Ray{sunray::create_point(1, 2, 3), sunray::create_vector(0, 1, 0)};
    sunray::Transformation trans;
    auto r = ray.transform(trans.translate(3, 4, 5).matrix());
    CHECK(r.origin() == sunray::create_point(4, 6, 8));
    CHECK(r.direction() == sunray::create_vector(0, 1, 0));
  }
  SECTION("scale ray")
  {
    auto ray = sunray::Ray{sunray::create_point(1, 2, 3), sunray::create_vector(0, 1, 0)};
    sunray::Transformation trans;
    auto r = ray.transform(trans.scale(2, 3, 4).matrix());
    CHECK(r.origin() == sunray::create_point(2, 6, 12));
    CHECK(r.direction() == sunray::create_vector(0, 3, 0));
  }
}
