//
//  cube_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 13.04.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/cube.h>
#include <sun_ray/feature/transformation.h>

#include <catch2/catch.hpp>


TEST_CASE("create cube", "[cube]")
{
  sunray::Material default_material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f};

  SECTION("create default cube")
  {
    auto cube = sunray::Cube::make_cube();
    CHECK(cube->origin() == sunray::create_point(0, 0, 0));
    CHECK(cube->transformation() == sunray::Matrix44::identity());
    CHECK(cube->inverse_transformation() == sunray::Matrix44::identity().inverse());
    CHECK(cube->material() == default_material);
  }
  SECTION("create sphere with material and transformation")
  {
    auto cube = sunray::Cube::make_cube(default_material, sunray::Matrix44::translation(2, 3, 4));
    CHECK(cube->origin() == sunray::create_point(0, 0, 0));
    CHECK(cube->transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(cube->inverse_transformation() == cube->transformation().inverse());
    CHECK(cube->material() == default_material);
  }
}

TEST_CASE("cube normal", "[cube]")
{
  SECTION("cube normal")
  {
    auto cube = sunray::Cube::make_cube();
    CHECK(cube->normal_at(sunray::create_point(1, 0.5, -0.8)) == sunray::create_vector(1, 0, 0));
    CHECK(cube->normal_at(sunray::create_point(-1, -0.2, 0.9)) == sunray::create_vector(-1, 0, 0));
    CHECK(cube->normal_at(sunray::create_point(-0.4, 1, -0.1)) == sunray::create_vector(0, 1, 0));
    CHECK(cube->normal_at(sunray::create_point(0.3, -1, -0.7)) == sunray::create_vector(0, -1, 0));
    CHECK(cube->normal_at(sunray::create_point(-0.6, 0.3, 1)) == sunray::create_vector(0, 0, 1));
    CHECK(cube->normal_at(sunray::create_point(0.4, 0.4, -1)) == sunray::create_vector(0, 0, -1));
    CHECK(cube->normal_at(sunray::create_point(1, 1, 1)) == sunray::create_vector(1, 0, 0));
    CHECK(cube->normal_at(sunray::create_point(-1, -1, -1)) == sunray::create_vector(-1, 0, 0));
  }
}

TEST_CASE("cube side intersections", "[cube]")
{
  auto cube = sunray::Cube::make_cube();

  SECTION("intersect x")
  {
    auto ray = sunray::Ray{sunray::create_point(5, 0.5, 0), sunray::create_vector(-1, 0, 0)};

    sunray::Intersections intersections;
    CHECK(cube->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].time() == Approx(4));
    CHECK(intersections.intersections()[1].time() == Approx(6));
  }
  SECTION("intersect -x")
  {
    auto ray = sunray::Ray{sunray::create_point(-5, 0.5, 0), sunray::create_vector(1, 0, 0)};

    sunray::Intersections intersections;
    CHECK(cube->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].time() == Approx(4));
    CHECK(intersections.intersections()[1].time() == Approx(6));
  }
  SECTION("intersect y")
  {
    auto ray = sunray::Ray{sunray::create_point(0.5, 5, 0), sunray::create_vector(0, -1, 0)};

    sunray::Intersections intersections;
    CHECK(cube->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].time() == Approx(4));
    CHECK(intersections.intersections()[1].time() == Approx(6));
  }
  SECTION("intersect -y")
  {
    auto ray = sunray::Ray{sunray::create_point(0.5, -5, 0), sunray::create_vector(0, 1, 0)};

    sunray::Intersections intersections;
    CHECK(cube->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].time() == Approx(4));
    CHECK(intersections.intersections()[1].time() == Approx(6));
  }
  SECTION("intersect z")
  {
    auto ray = sunray::Ray{sunray::create_point(0.5, 0, 5), sunray::create_vector(0, 0, -1)};

    sunray::Intersections intersections;
    CHECK(cube->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].time() == Approx(4));
    CHECK(intersections.intersections()[1].time() == Approx(6));
  }
  SECTION("intersect -z")
  {
    auto ray = sunray::Ray{sunray::create_point(0.5, 0, -5), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    CHECK(cube->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].time() == Approx(4));
    CHECK(intersections.intersections()[1].time() == Approx(6));
  }
  SECTION("intersect inside")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0.5, 0), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    CHECK(cube->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].time() == Approx(-1));
    CHECK(intersections.intersections()[1].time() == Approx(1));
  }
}

TEST_CASE("cube intersection misses", "[cube]")
{
  auto cube = sunray::Cube::make_cube();

  SECTION("side misses")
  {
    sunray::Intersections intersections;
    CHECK_FALSE(cube->is_intersected_by(
      sunray::Ray{sunray::create_point(-2, 0, 0), sunray::create_vector(0.2673, 0.5345, 0.8018)}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cube->is_intersected_by(
      sunray::Ray{sunray::create_point(0, -2, 0), sunray::create_vector(0.8018, 0.2673, 0.5345)}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cube->is_intersected_by(
      sunray::Ray{sunray::create_point(0, 0, -2), sunray::create_vector(0.5345, 0.8018, 0.2673)}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(
      cube->is_intersected_by(sunray::Ray{sunray::create_point(2, 0, 2), sunray::create_vector(0, 0, -1)}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(
      cube->is_intersected_by(sunray::Ray{sunray::create_point(0, 2, 2), sunray::create_vector(0, -1, 0)}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(
      cube->is_intersected_by(sunray::Ray{sunray::create_point(2, 2, 0), sunray::create_vector(-1, 0, 0)}, intersections));
    CHECK(intersections.intersections().empty());
  }
}
