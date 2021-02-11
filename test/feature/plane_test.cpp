//
//  plane_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 04.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/plane.h>
#include <sun_ray/feature/transformation.h>

#include <catch2/catch.hpp>


TEST_CASE("create plane", "[plane]")
{
  sunray::Material default_material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f};

  SECTION("create default plane")
  {
    auto plane = sunray::Plane::make_plane();
    CHECK(plane->origin() == sunray::create_point(0, 0, 0));
    CHECK(plane->transformation() == sunray::Matrix44::identity());
    CHECK(plane->inverse_transformation() == sunray::Matrix44::identity().inverse());
    CHECK(plane->material() == default_material);
  }
  SECTION("create sphere with material and transformation")
  {
    auto plane = sunray::Plane::make_plane(default_material, sunray::Matrix44::translation(2, 3, 4));
    CHECK(plane->origin() == sunray::create_point(0, 0, 0));
    CHECK(plane->transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(plane->inverse_transformation() == plane->transformation().inverse());
    CHECK(plane->material() == default_material);
  }
}

TEST_CASE("plane normal", "[plane]")
{
  SECTION("plane normal")
  {
    auto plane = sunray::Plane::make_plane();
    CHECK(plane->normal_at(sunray::create_point(0, 0, 0)) == sunray::create_vector(0, 1, 0));
    CHECK(plane->normal_at(sunray::create_point(10, 0, -10)) == sunray::create_vector(0, 1, 0));
    CHECK(plane->normal_at(sunray::create_point(-5, 0, 150)) == sunray::create_vector(0, 1, 0));
  }
}

TEST_CASE("plane intersections", "[plane]")
{
  SECTION("intersect parallel")
  {
    auto plane = sunray::Plane::make_plane();
    auto ray = sunray::Ray{sunray::create_point(0, 10, 0), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    CHECK_FALSE(plane->is_intersected_by(ray, intersections));
  }
  SECTION("intersect coplanar")
  {
    auto plane = sunray::Plane::make_plane();
    auto ray = sunray::Ray{sunray::create_point(0, 0, 0), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    CHECK_FALSE(plane->is_intersected_by(ray, intersections));
  }
  SECTION("intersect from above")
  {
    auto plane = sunray::Plane::make_plane();
    auto ray = sunray::Ray{sunray::create_point(0, 1, 0), sunray::create_vector(0, -1, 0)};

    sunray::Intersections intersections;
    CHECK(plane->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 1);
    CHECK(intersections.intersections()[0].time() == Approx(1));
    CHECK(plane.get() == intersections.intersections()[0].object());
  }
  SECTION("intersect from below")
  {
    auto plane = sunray::Plane::make_plane();
    auto ray = sunray::Ray{sunray::create_point(0, -1, 0), sunray::create_vector(0, 1, 0)};

    sunray::Intersections intersections;
    CHECK(plane->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 1);
    CHECK(intersections.intersections()[0].time() == Approx(1));
    CHECK(plane.get() == intersections.intersections()[0].object());
  }
}
